#include "gdrive/oauth.hpp"
#include "gdrive/request.hpp"

namespace GDRIVE {

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
    request.get_response();
    return true;
}

}
