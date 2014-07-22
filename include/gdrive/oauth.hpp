#ifndef __GDRIVE_OAUTH_HPP__
#define __GDRIVE_OAUTH_HPP__

#include <string>
#include "gdrive/config.hpp"
#include "gdrive/util.hpp"
#include "gdrive/logging.hpp"

#define OAUTH_URL "https://accounts.google.com/o/oauth2/auth"
#define REVOKE_URL "https://accounts.google.com/o/oauth2/revoke"
#define TOKEN_URL "https://accounts.google.com/o/oauth2/token"
#define SCOPE "https://www.googleapis.com/auth/drive"
#define REDIRECT_URI "urn:ietf:wg:oauth:2.0:oob"
#define RESPONSE_TYPE "code"
#define ACCESS_TYPE "offline"
#define USER_AGENT "allenbo-gdrive/1.0"

namespace GDRIVE {

class OAuth {
    CLASS_MAKE_LOGGER
    public:
        OAuth(std::string client_id, std::string client_secret);

        std::string get_authorize_url();
        bool build_credential(std::string code);
   
    private:
        std::string _client_id;
        std::string _client_secret;
        std::string _code;
};

}

#endif
