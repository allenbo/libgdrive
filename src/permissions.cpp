#include "gdrive/service/permissions.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

PermissionService PermissionService::_single_instance;

PermissionService::PermissionService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("PermissionService", L_DEBUG)
#endif
}

PermissionListRequest PermissionService::List(std::string file_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/permissions");
    PermissionListRequest plr(_cred, vs.toString());
    return plr;
}

PermissionGetRequest PermissionService::Get(std::string file_id, std::string permission_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/permissions/").append(permission_id);
    PermissionGetRequest pgr(_cred, vs.toString());
    return pgr;
}

PermissionInsertRequest PermissionService::Insert(std::string file_id, GPermission* permission) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/permissions");
    PermissionInsertRequest pir(permission, _cred, vs.toString());
    return pir;
}

PermissionDeleteRequest PermissionService::Delete(std::string file_id, std::string permission_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/permissions/").append(permission_id);
    PermissionDeleteRequest pdr(_cred, vs.toString());
    return pdr;
}

PermissionPatchRequest PermissionService::Patch(std::string file_id, std::string permission_id, GPermission* permission) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/permissions/").append(permission_id);
    PermissionPatchRequest ppr(permission, _cred, vs.toString());
    return ppr;
}

PermissionUpdateRequest PermissionService::Update(std::string file_id, std::string permission_id, GPermission* permission) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/permissions/").append(permission_id);
    PermissionUpdateRequest pur(permission, _cred, vs.toString());
    return pur;
}

PermissionGetIdForEmailRequest PermissionService::GetIdForEmail(std::string email) {
    VarString vs;
    vs.append(SERVICE_URI).append("/permissionIds/").append(email);
    PermissionGetIdForEmailRequest pgr(_cred, vs.toString());
    return pgr;
}


}
