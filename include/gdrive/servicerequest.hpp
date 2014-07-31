#ifndef __GDRIVE_SERVICEREQUEST_HPP__
#define __GDRIVE_SERVICEREQUEST_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/logging.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"

#include <vector>

#define FILE_URL SERVICE_URI "/files"

namespace GDRIVE {

class FieldRequest: public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        FieldRequest(Credential* cred, std::string uri, RequestMethod method)
            :CredentialHttpRequest(cred, uri, method) {}
        inline void clear_fields() {
            if (_query.find("fields") == _query.end()) return;
            _query.erase("fields");
        }

        inline void add_field(std::string field) {
            if (_query.find("fields") == _query.end()) {
                _query["fields"] = field;
            } else {
                _query["fields"] += "," + field;
            }
        };
    protected: 
        GFile get_file();
};

class FileListRequest: public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        FileListRequest(Credential* cred, std::string uri, RequestMethod method)
            :CredentialHttpRequest(cred, uri, method) {}
        std::vector<GFile> execute(); 
        void set_corpus(std::string corpus);
        void set_max_results(int max_results);
        void set_page_token(std::string page_token);
        void set_q(std::string q);
};

class FileGetRequest: public FieldRequest {
    CLASS_MAKE_LOGGER
    public:
        FileGetRequest(Credential* cred, std::string uri, RequestMethod method)
            :FieldRequest(cred, uri, method) {}
        GFile execute();
        void set_update_viewed_date(bool);
};

class FileTrashRequest: public FieldRequest {
    CLASS_MAKE_LOGGER
    public:
        FileTrashRequest(Credential* cred, std::string uri, RequestMethod method)
            :FieldRequest(cred, uri, method) {}
        GFile execute();
};

typedef FileTrashRequest FileUntrashRequest;

class FileDeleteRequest: public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        FileDeleteRequest(Credential* cred, std::string uri, RequestMethod method)
            :CredentialHttpRequest(cred, uri, method) {}
        bool execute();
};

typedef FileDeleteRequest FileEmptyTrashRequest;

}

#endif
