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

class DriveService {
    CLASS_MAKE_LOGGER
    public:
        DriveService(Credential cred);
    protected:
        Credential _cred;
};

class FileService : public DriveService {
    CLASS_MAKE_LOGGER
    public:
        FileService(Credential cred);
        std::vector<GFile> List();
        GFile Get(std::string id);
        GFile Trash(std::string id);
        GFile Untrash(std::string id);
        bool Delete(std::string id);
        bool EmptyTrash();
};

}

#endif
