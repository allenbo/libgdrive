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
        FieldRequest(Credential* cred, std::string uir, RequestMethod method);
        inline void clear_fields();
        inline void add_field(std::string field);
};

class FileListRequest: public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        FileListRequest(Credential* cred, std::string uri, RequestMethod method);
        std::vector<GFile> execute(); 
        void set_corpus(std::string corpus);
        void set_max_results(int max_results);
        void set_page_token(std::string page_token);
        void set_q(std::string q);
};


/*
class FileRequest: public ServiceRequest {
    CLASS_MAKE_LOGGER
    public:
        FileRequest(GFile file, Credential* cred, std::string uri, RequestMethod method);
        void add_field(std::string field) {
            _fields.push_back(field);
        }
    protected:
        GFile _file;
        std::string _json_encode_body();
        std::vector<std::string> _fields;
};
*/

}

#endif
