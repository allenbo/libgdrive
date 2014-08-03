#include "gdrive/changeservice.hpp"
#include "jconer/json.hpp"

#include <string.h>
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
    vs.append(CHANGE_URL).append('/').append(id);
    ChangeGetRequest cgr(_cred, vs.toString());
    return cgr;
}

}
