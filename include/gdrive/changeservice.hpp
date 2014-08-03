#ifndef __GDRIVE_CHANGESERVICE_HPP__
#define __GDRIVE_CHANGESERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/logging.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"

#include <vector>

namespace GDRIVE {

class ChangeService {
    CLASS_MAKE_LOGGER
    public:
        static ChangeService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }

    private:
        ChangeService();
        ChangeService(const ChangeService& other);
        ChangeService& operator=(const ChangeService& other);

        static ChangeService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }
};

}
#endif
