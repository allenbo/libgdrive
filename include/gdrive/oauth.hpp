#ifndef __GDRIVE_OAUTH_HPP__
#define __GDRIVE_OAUTH_HPP__

#include <string>
#include "gdrive/util.hpp"

#define OAUTH_URL "https://accounts.google.com/o/oauth2/auth"
#define SCOPE "https://www.googleapis.com/auth/drive"
#define REDIRECT_URI "urn:ietf:wg:oauth:2.0:oob"
#define RESPONSE_TYPE "code"
#define ACCESS_TYPE "offline"

namespace GDRIVE {

class OAuth {
    public:
        OAuth(std::string client_id, std::string client_secret)
            :_client_id(client_id), _client_secret(client_secret) {}

        std::string get_authorize_url();
   
    private:
        std::string _client_id;
        std::string _client_secret;
};

}

#endif
