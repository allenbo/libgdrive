#ifndef __GDRIVE_DRIVE_HPP__
#define __GDRIVE_DRIVE_HPP__

#include "gdrive/util.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/config.hpp"
#include "gdrive/fileservice.hpp"
#include "gdrive/aboutservice.hpp"
#include "gdrive/changeservice.hpp"
#include "common/all.hpp"

#include <vector>


namespace GDRIVE {

class Drive {
    CLASS_MAKE_LOGGER
    public:
        Drive(Credential* cred);
        FileService& files();
        AboutService& about();
        ChangeService& changes();
    protected:
        Credential* _cred;
};


}

#endif
