#ifndef __GDRIVE_OAUTH_HPP__
#define __GDRIVE_OAUTH_HPP__

#include <string>
#include "gdrive/config.hpp"
#include "gdrive/util.hpp"
#include "gdrive/logging.hpp"
#include "gdrive/credential.hpp"

namespace GDRIVE {

class OAuth {
    CLASS_MAKE_LOGGER
    public:
        OAuth(std::string client_id, std::string client_secret);

        std::string get_authorize_url();
        bool build_credential(std::string code, Credential& cred);
   
    private:
        std::string _client_id;
        std::string _client_secret;
        std::string _code;

        void _parse_response(std::string content);
        struct CredentialResponse {
            CredentialResponse() {
                access_token = "";
                token_type = "";
                refresh_token = "";
                id_token = "";
                expires_in = 0;
                token_expiry = 0;
            }

            std::string access_token;
            std::string token_type;
            std::string refresh_token;
            std::string id_token;
            long expires_in;
            long token_expiry;
        };

        CredentialResponse _resp;
        
};

}

#endif
