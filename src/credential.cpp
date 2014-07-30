#include "gdrive/credential.hpp"
#include "jconer/json.hpp"

using namespace JCONER;

namespace GDRIVE {

Credential::Credential(Store* store)
    :_store(store)
{
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("Credential", L_DEBUG);
#endif
    if (_store->get("refresh_token") == "") {
        _invalid = true;
        _access_token = "";
        _client_id = _store->get("client_id");
        _client_secret = _store->get("client_secret");
        _refresh_token = "";
        _id_token = "";
    } else {
        _invalid = false;
        _access_token = _store->get("access_token");
        _client_id = _store->get("client_id");
        _client_secret = _store->get("client_secret");
        _refresh_token = _store->get("refresh_token");
        _id_token = _store->get("id_token");
    }
    _token_expiry = 0;
}

void Credential::dump() {
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

CredentialHttpRequest::CredentialHttpRequest(Credential* cred, std::string uri, RequestMethod method)
    :HttpRequest(uri, method), _cred(cred)
{
}

void CredentialHttpRequest::_apply_header() {
    add_header("Authorization", "Bearer " + _cred->_access_token);
    add_header("user-agent", USER_AGENT);
}

std::string CredentialHttpRequest::_generate_request_body() {
    std::map<std::string, std::string> body;
    body["grant_type"] = "refresh_token";
    body["client_id"] = _cred->_client_id;
    body["client_secret"] = _cred->_client_secret;
    body["refresh_token"] = _cred->_refresh_token;
    return URLHelper::encode(body);
}

RequestHeader CredentialHttpRequest::_generate_request_header() {
    RequestHeader header;
    header["User-Agent"] = USER_AGENT;
    header["Content-Type"] = "application/x-www-form-urlencoded";
    return header;
}

void CredentialHttpRequest::_parse_response(std::string content) {
    PError perr;
    JObject* rst = (JObject*)loads(content, perr);
    if (rst != NULL){
        if (rst->contain("access_token")) {
            _cred->_access_token = ((JString*)rst->get("access_token"))->getValue();
        }
        if (rst->contain("refresh_token")) {
            _cred->_refresh_token = ((JString*)rst->get("refresh_token"))->getValue();
        }
        if (rst->contain("expire_in")) {
            long expire_in = ((JInt*)rst->get("expire_in"))->getValue();
            _cred->_token_expiry = (long)time(NULL) + expire_in;
        } else {
            _cred->_token_expiry = 0;
        }
        delete rst;
    }
    _cred->dump();
}

void CredentialHttpRequest::_refresh() {
    RequestHeader header = _generate_request_header(); 
    std::string body = _generate_request_body(); 
    
    HttpRequest request(TOKEN_URL, RM_POST, header, body);
    HttpResponse resp = request.request();

    if (resp.status() == 200) {
        _parse_response(resp.content());
    } else {
        CLOG_ERROR("error_msg:%s\n", resp.content().c_str());
    }
}

HttpResponse CredentialHttpRequest::request() {
    if (_cred->_access_token == ""){
        CLOG_INFO("Attempting refresh to obtain initial access_token\n");
        _refresh();
    }

    _apply_header();
    HttpRequest::request();

    if (_resp.status() == 401) {
        CLOG_INFO("Need to refresh\n");
        _resp.clear();
        _refresh();
        _apply_header();
        HttpRequest::request();
    }
    return _resp;
}

}
