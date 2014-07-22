#include "gdrive/oauth.hpp"
#include "gdrive/request.hpp"
#include "jconer/json.hpp"
#include <time.h>

using namespace JCONER;

namespace GDRIVE {
OAuth::OAuth(std::string client_id, std::string client_secret)
    :_client_id(client_id), _client_secret(client_secret)
{
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("OAuth", L_DEBUG)
#endif
}

std::string OAuth::get_authorize_url() {
    VarString vs;
    vs.append(OAUTH_URL, '?')
      .append("scope", '=', URLHelper::encode(SCOPE)).append('&')
      .append("redirect_uri", '=', URLHelper::encode(REDIRECT_URI)).append('&')
      .append("response_type", '=', URLHelper::encode(RESPONSE_TYPE)).append('&')
      .append("client_id", '=', URLHelper::encode(_client_id.c_str())).append('&')
      .append("access_type", '=', URLHelper::encode(ACCESS_TYPE));

    return vs.toString();
}

bool OAuth::build_credential(std::string code) {
    _code = code;
    RequestBody body;
    body["grant_type"] = "authorization_code";
    body["client_id"] = _client_id;
    body["client_secret"] = _client_secret;
    body["code"] = _code;
    body["redirect_uri"] = REDIRECT_URI;
    body["scope"] = SCOPE;

    RequestHeader header;
    header["content-type"] = "application/x-www-form-urlencoded";
    header["user-agent"] = USER_AGENT;

    Request request(TOKEN_URL, RM_POST, body, header);
    request.request();
    Response resp = request.response();

    CLOG_DEBUG("Response:%s\n", resp.content().c_str());
    
    PError perr;
    JObject* rst = (JObject*)loads(resp.content(), perr);
    std::map<std::string, std::string> token_resp;
    long expire_in = 0;
    if (rst != NULL) {
        std::vector<std::string> keys = rst->getKeys();
        for(std::vector<std::string>::iterator iter = keys.begin(); iter != keys.end(); iter ++) {
            JValue* value = rst->get(*iter);
            if (value->type() == VT_INTEGER) {
                expire_in = ((JInt*)value)->getValue();
                CLOG_DEBUG("Get expire_in: %d\n", expire_in);
            } else {
                token_resp[*iter] = ((JString*)value)->getValue();
                CLOG_DEBUG("Get %s: %s\n", iter->c_str(), token_resp[*iter].c_str());
            }
        }
        delete rst;
    }

    if (resp.status() == 200 && rst != NULL) {
        std::string access_token = token_resp["access_token"];
        std::string refresh_token = token_resp["refresh_token"];
        long token_expiry = (long)time(NULL) + expire_in;
        CLOG_DEBUG("access_token:%s\n", access_token.c_str());
        CLOG_DEBUG("refresh_token:%s\n", refresh_token.c_str());
        CLOG_DEBUG("token_expiry:%d\n", token_expiry); 
        return true;
    }
    else {
        return false;
    }
}

}
