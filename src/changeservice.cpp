#include "gdrive/changeservice.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

ChangeService ChangeService::_single_instance;

ChangeService::ChangeService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("FileService", L_DEBUG)
#endif
}

ChangeGetRequest ChangeService::Get(std::string id) {
    VarString vs;
    vs.append(CHANGES_URL).append('/').append(id);
    ChangeGetRequest cgr(_cred, vs.toString());
    return cgr;
}

ChangeListRequest ChangeService::List() {
    VarString vs;
    vs.append(CHANGES_URL);
    ChangeListRequest clr(_cred, vs.toString());
    return clr;
}

std::vector<GChange> ChangeService::Listall() {
    ChangeListRequest list = List();
    std::vector<GChange> changes;
    while(true) {
        GChangeList changelist = list.execute();
        list.clear();
        const std::vector<GChange>& tmp = changelist.get_items();
        changes.insert(changes.end(), tmp.begin(), tmp.end());
        std::string pageToken = changelist.get_nextPageToken();
        if (pageToken == "") {
            break;
        } else {
            list.set_pageToken(pageToken);
        }
    }
    return changes;
}

}
