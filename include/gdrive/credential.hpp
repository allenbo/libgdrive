#ifndef __GDRIVE_CREDENTIAL_HPP__
#define __GDRIVE_CREDENTIAL_HPP__

#include "gdrive/config.hpp"
#include "gdrive/util.hpp"
#include "gdrive/request.hpp"
#include "gdrive/store.hpp"
#include "common/all.hpp"

#include <string>

namespace GDRIVE {

class CredentialHttpRequest;

class Credential {
    CLASS_MAKE_LOGGER
    public:
        Credential(Store* store);
        inline bool invalid() const { return _invalid; }
        void refresh(std::string at, std::string rt, long te, std::string it = "");
        void dump();
    private:
        std::string _access_token;
        std::string _client_id;
        std::string _client_secret;
        std::string _refresh_token;
        long _token_expiry;
        std::string _id_token;
        bool _invalid;

        Store *_store;

        Credential(const Credential& other);
        Credential& operator=(const Credential& other);

    friend class CredentialHttpRequest;
};


class CredentialHttpRequest : public HttpRequest {
    CLASS_MAKE_LOGGER
    public:
        CredentialHttpRequest(Credential *cred, std::string uri, RequestMethod method);
        HttpResponse request();
    protected:
        Credential *_cred;

        void _apply_header();
        void _refresh();

        std::string _generate_request_body();
        RequestHeader _generate_request_header();

        void _parse_response(std::string content);
};

}

#endif
