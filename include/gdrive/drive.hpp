#ifndef __GDRIVE_DRIVE_HPP__
#define __GDRIVE_DRIVE_HPP__

#include "gdrive/logging.hpp"
#include "gdrive/util.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/config.hpp"

#include <vector>

#define SERVICE_URI "https://www.googleapis.com/drive/v2"
#define FILE_URL SERVICE_URI "/files"

namespace GDRIVE {

class FileService;

class Drive {
    CLASS_MAKE_LOGGER
    public:
        Drive(Credential* cred);
        FileService& files();
    protected:
        Credential* _cred;
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
class FileService {
    CLASS_MAKE_LOGGER
    public:
        static FileService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }
        FileListRequest List();
        /*
        GFile Get(std::string id);
        GFile Trash(std::string id);
        GFile Untrash(std::string id);
        bool Delete(std::string id);
        bool EmptyTrash();
        GFile Touch(std::string id);
        PatchRequest Patch(std::string file_id, GFile file);
        */
    private: 
        FileService();
        FileService(const FileService& other);
        FileService& operator=(const FileService& other);
        static FileService _single_instance;

        Credential *_cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }
};

}

#endif
