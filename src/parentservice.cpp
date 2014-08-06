#include "gdrive/parentservice.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

ParentService ParentService::_single_instance;

ParentService::ParentService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("ParentService", L_DEBUG)
#endif
}

ParentListRequest ParentService::List(std::string file_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/parents");
    ParentListRequest plr(_cred, vs.toString());
    return plr;
}

ParentGetRequest ParentService::Get(std::string file_id, std::string parent_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/parents/").append(parent_id);
    ParentGetRequest pgr(_cred, vs.toString());
    return pgr;
}

ParentInsertRequest ParentService::Insert(std::string file_id, GParent* parent) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/parents");
    ParentInsertRequest pir(parent, _cred, vs.toString());
    return pir;
}

ParentDeleteRequest ParentService::Delete(std::string file_id, std::string parent_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/parents/").append(parent_id);
    ParentDeleteRequest pdr(_cred, vs.toString());
    return pdr;
}

}
