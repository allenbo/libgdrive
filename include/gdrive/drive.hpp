#ifndef __GDRIVE_DRIVE_HPP__
#define __GDRIVE_DRIVE_HPP__

#include "gdrive/logging.hpp"
#include "gdrive/util.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/config.hpp"
#include "gdrive/fileservice.hpp"
#include "gdrive/aboutservice.hpp"

#include <vector>


namespace GDRIVE {

class Drive {
    CLASS_MAKE_LOGGER
    public:
        Drive(Credential* cred);
        FileService& files();
        AboutService& about();
    protected:
        Credential* _cred;
};


}

#endif
