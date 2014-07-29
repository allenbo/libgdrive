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
        Drive(Credential cred);
        FileService& files();
    protected:
        Credential _cred;
};

class FileService {
    CLASS_MAKE_LOGGER
    public:
        class PatchRequest : public Request {
            CLASS_MAKE_LOGGER
            public:
                PatchRequest(std::string uri, std::string file_id, GFile file);
                void add_field(std::string field) {
                    _fields.push_back(field);
                }
                GFile execute();
            private:
                std::string _file_id;
                GFile _file;
                std::string _jsonencode_body();
                std::vector<std::string> _fields;
        };
        static FileService& get_instance(Credential &cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }
        std::vector<GFile> List();
        GFile Get(std::string id);
        GFile Trash(std::string id);
        GFile Untrash(std::string id);
        bool Delete(std::string id);
        bool EmptyTrash();
        GFile Touch(std::string id);
        PatchRequest Patch(std::string file_id, GFile file);
    private: 
        FileService();
        FileService(const FileService& other);
        FileService& operator=(const FileService& other);
        static FileService _single_instance;

        Credential _cred;
        inline void set_cred(Credential& cred) {
            _cred = cred;
        }
};

}

#endif
