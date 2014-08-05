#ifndef __GDRIVE_ABOUTSERVICE_HPP__
#define __GDRIVE_ABOUTSERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "common/all.hpp"

#include <vector>

namespace GDRIVE {

class AboutService {
    CLASS_MAKE_LOGGER
    public:
        static AboutService& get_instance(Credential* cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }

        AboutGetRequest Get();
    private:
        AboutService();
        AboutService(const AboutService& other);
        AboutService& operator=(const AboutService& other);

        static AboutService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }
};

}

#endif
