#include "gdrive/credential.hpp"
#include "jconer/json.hpp"

using namespace JCONER;

namespace GDRIVE {

Credential::Credential(std::string access_token, std::string client_id,
                   std::string client_secret, std::string refresh_token,
                   long token_expiry, std::string id_token) {
 #ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("Credential", L_DEBUG);
#endif
    _access_token = access_token;
    _client_id = client_id;
    _client_secret = client_secret;
    _refresh_token = refresh_token;
    _token_expiry = token_expiry;
    _token_uri = TOKEN_URL;
    _user_agent = USER_AGENT;
    _revoke_uri = REVOKE_URL;
    _id_token = id_token;
    _invalid = false;
    _store = NULL;
}

Credential::Credential() {
    _access_token = "";
    _client_id = "";
    _client_secret = "";
    _refresh_token = "";
    _token_expiry = 0;
    _token_uri = TOKEN_URL;
    _user_agent = USER_AGENT;
    _revoke_uri = REVOKE_URL;
    _id_token = "";
    _invalid = true;
    _store = NULL;

#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("Credential", L_DEBUG);
#endif   
}

Credential::Credential(const Credential& other) {
    _access_token = other._access_token;
    _client_id = other._client_id;
    _client_secret = other._client_secret;
    _refresh_token = other._refresh_token;
    _token_expiry = other._token_expiry;
    _token_uri = other._token_uri;
    _user_agent = other._user_agent;
    _revoke_uri = other._revoke_uri;
    _id_token = other._id_token;
    _invalid = other._invalid;
    _store = other._store;

#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("Credential", L_DEBUG);
#endif
}

void Credential::_apply_header(Request& req) {
    req.add_header("Authorization", "Bearer " + _access_token);
    req.add_header("user-agent", _user_agent);
}

RequestBody Credential::_generate_request_body() {
    RequestBody body;
    body["grant_type"] = "refresh_token";
    body["client_id"] = _client_id;
    body["client_secret"] = _client_secret;
    body["refresh_token"] = _refresh_token;
    return body;
}

RequestHeader Credential::_generate_request_header() {
    RequestHeader header;
    header["User-Agent"] = _user_agent;
    header["Content-Type"] = "application/x-www-form-urlencoded";
    return header;
}

void Credential::_parse_response(std::string content) {
    PError perr;
    JObject* rst = (JObject*)loads(content, perr);
    if (rst != NULL){
        if (rst->contain("access_token")) {
            _access_token = ((JString*)rst->get("access_token"))->getValue();
        }
        if (rst->contain("refresh_token")) {
            _refresh_token = ((JString*)rst->get("refresh_token"))->getValue();
        }
        if (rst->contain("expire_in")) {
            long expire_in = ((JInt*)rst->get("expire_in"))->getValue();
            _token_expiry = (long)time(NULL) + expire_in;
        } else {
            _token_expiry = 0;
        }
        delete rst;
    }
}

void Credential::_fresh() {
    if (_store == NULL) {
        CLOG_WARN("This is no store to save tokens\n");
        return;
    }
    _store->put("access_token", _access_token);
    _store->put("client_id", _client_id);
    _store->put("client_secret", _client_secret);
    _store->put("refresh_token", _refresh_token);
    _store->put("id_token", _id_token);
    _store->dump(); 
}

void Credential::from_store(Store* store) {
    _store = store;
    _invalid = false;
    _access_token = _store->get("access_token");
    _client_id = _store->get("client_id");
    _client_secret = _store->get("client_secret");
    _refresh_token = _store->get("refresh_token");
    _id_token = _store->get("id_token");
}

void Credential::_refresh() {
    RequestHeader header = _generate_request_header(); 
    RequestBody body = _generate_request_body(); 
    
    Request request(_token_uri, RM_POST, body, header);
    request.request();
    Response resp = request.response();

    if (resp.status() == 200) {
        _parse_response(resp.content());
        _fresh();
    } else {
        CLOG_ERROR("error_msg:%s\n", resp.content().c_str());
    }
}

Response Credential::request(Request& req) {
    if (_access_token == ""){
        CLOG_INFO("Attempting refresh to obtain initial access_token\n");
        _refresh();
    }

    _apply_header(req);
    CLOG_DEBUG("Apply header to request\n");
    
    req.request();
    Response resp = req.response();
    if (resp.status() == 401) {
        CLOG_INFO("Need to refresh\n");
        _refresh();
        _apply_header(req);
        req.request();
        return req.response();
    } else {
        return resp;
    }
}

}
