#include "gdrive/service/apps.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

AppService AppService::_single_instance;

AppService::AppService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("AppService", L_DEBUG)
#endif
}

AppListRequest AppService::List() {
    VarString vs;
    vs.append(APPS_URL);
    AppListRequest r(_cred, vs.toString());
    return r;
}

AppGetRequest AppService::Get(std::string app_id) {
    VarString vs;
    vs.append(APPS_URL).append('/').append(app_id);
    AppGetRequest r(_cred, vs.toString());
    return r;
}

}
