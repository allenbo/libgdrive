#include "gdrive/childrenservice.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

ChildrenService ChildrenService::_single_instance;

ChildrenService::ChildrenService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("FileService", L_DEBUG)
#endif
}

ChildrenListRequest ChildrenService::List(std::string folder_id) {
    VarString vs;
    vs.append(FILES_URL).append("/").append(folder_id).append("/children");
    ChildrenListRequest clr(_cred, vs.toString());
    return clr;
}

std::vector<GChildren> ChildrenService::Listall(std::string folder_id){
    ChildrenListRequest list = List(folder_id);
    std::vector<GChildren> children;
    while(true) {
        GChildrenList childrenlist = list.execute();
        list.clear();
        const std::vector<GChildren>& tmp = childrenlist.get_items();
        children.insert(children.end(), tmp.begin(), tmp.end());
        std::string pageToken = childrenlist.get_nextPageToken();
        if (pageToken == "") {
            break;
        } else {
            list.set_pageToken(pageToken);
        }
    }
    return children;
}

ChildrenGetRequest ChildrenService::Get(std::string folder_id, std::string child_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(folder_id).append("/children/").append(child_id);
    ChildrenGetRequest cgr(_cred, vs.toString());
    return cgr;
}

ChildrenInsertRequest ChildrenService::Insert(std::string folder_id, GChildren* child) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(folder_id).append("/children");
    ChildrenInsertRequest cir(child, _cred, vs.toString());
    return cir;
}

}
