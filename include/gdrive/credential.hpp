#ifndef __GDRIVE_CREDENTIAL_HPP__
#define __GDRIVE_CREDENTIAL_HPP__

#include "gdrive/logging.hpp"
#include "gdrive/config.hpp"
#include "gdrive/util.hpp"
#include "gdrive/request.hpp"

#include <string>

namespace GDRIVE {

class Credential {
    CLASS_MAKE_LOGGER
    public:
        Credential(std::string access_token, std::string client_id,
                   std::string client_secret, std::string refresh_token,
                   long token_expiry, std::string token_uri,
                   std::string user_agent, std::string revoke_uri,
                   std::string id_token);
        Credential(const Credential& other);
        Credential();
        
        Response request(Request&);
    private:
        std::string _access_token;
        std::string _client_id;
        std::string _client_secret;
        std::string _refresh_token;
        long _token_expiry;
        std::string _token_uri;
        std::string _user_agent;
        std::string _revoke_uri;
        std::string _id_token;
        bool _invalid;

        void _apply_header(Request& req);
        void _refresh();

        RequestBody _generate_request_body();
        RequestHeader _generate_request_header();

        void _parse_response(std::string content);
};

}

#endif
