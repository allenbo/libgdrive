#include "gdrive/service/revisions.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

RevisionService RevisionService::_single_instance;

RevisionService::RevisionService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("RevisionService", L_DEBUG)
#endif
}

RevisionListRequest RevisionService::List(std::string file_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/revisions");
    RevisionListRequest plr(_cred, vs.toString());
    return plr;
}

RevisionGetRequest RevisionService::Get(std::string file_id, std::string revision_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/revisions/").append(revision_id);
    RevisionGetRequest pgr(_cred, vs.toString());
    return pgr;
}

RevisionDeleteRequest RevisionService::Delete(std::string file_id, std::string revision_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/revisions/").append(revision_id);
    RevisionDeleteRequest pdr(_cred, vs.toString());
    return pdr;
}

RevisionPatchRequest RevisionService::Patch(std::string file_id, std::string revision_id, GRevision* revision) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/revisions/").append(revision_id);
    RevisionPatchRequest ppr(revision, _cred, vs.toString());
    return ppr;
}

RevisionUpdateRequest RevisionService::Update(std::string file_id, std::string revision_id, GRevision* revision) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/revisions/").append(revision_id);
    RevisionUpdateRequest pur(revision, _cred, vs.toString());
    return pur;
}

}
