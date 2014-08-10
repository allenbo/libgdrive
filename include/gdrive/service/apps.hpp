#ifndef __GDRIVE_APPSERVICE_HPP__
#define __GDRIVE_APPSERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "common/all.hpp"

#include <vector>

namespace GDRIVE {

class AppService {
    CLASS_MAKE_LOGGER
    public:
        static AppService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }
        
        AppListRequest List();
        AppGetRequest Get(std::string app_id);
    private:
        AppService();
        AppService(const AppService& other);
        AppService& operator=(const AppService& other);

        static AppService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }

};
}
#endif
