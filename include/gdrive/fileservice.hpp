#ifndef __GDRIVE_FILESERVICE_HPP__
#define __GDRIVE_FILESERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/logging.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "gdrive/filecontent.hpp"

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
        FileGetRequest Get(std::string id);
        FileTrashRequest Trash(std::string id);
        FileUntrashRequest Untrash(std::string id);
        FileDeleteRequest Delete(std::string id);
        FileEmptyTrashRequest EmptyTrash();
        FileTouchRequest Touch(std::string id);
        FilePatchRequest Patch(std::string file_id, GFile* file);
        FileCopyRequest Copy(std::string file_id, GFile* file);
        FileInsertRequest Insert(GFile* file, FileContent* content, bool resumable = false);
        FileUpdateRequest Update(std::string id, GFile* file, FileContent* content, bool resumable = false);
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
