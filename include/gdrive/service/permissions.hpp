#ifndef __GDRIVE_PERMISSIONSERVICE_HPP__
#define __GDRIVE_PERMISSIONSERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "common/all.hpp"

#include <vector>

namespace GDRIVE {

class PermissionService {
    CLASS_MAKE_LOGGER
    public:
        static PermissionService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }
        
        PermissionListRequest List(std::string file_id);
        PermissionGetRequest Get(std::string file_id, std::string permission_id);
        PermissionInsertRequest Insert(std::string file_id, GPermission* permission);
        PermissionDeleteRequest Delete(std::string file_id, std::string permission_id);
    private:
        PermissionService();
        PermissionService(const PermissionService& other);
        PermissionService& operator=(const PermissionService& other);

        static PermissionService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }

};
}
#endif
