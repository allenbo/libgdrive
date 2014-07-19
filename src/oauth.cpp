#include "gdrive/oauth.hpp"

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

}
