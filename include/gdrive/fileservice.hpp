#ifndef __GDRIVE_FILESERVICE_HPP__
#define __GDRIVE_FILESERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/logging.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "servicerequest.hpp"

#include <vector>

namespace GDRIVE {



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
