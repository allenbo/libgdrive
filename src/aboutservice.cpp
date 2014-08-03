#include "gdrive/aboutservice.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;

namespace GDRIVE {

AboutService AboutService::_single_instance;

AboutService::AboutService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("AboutService", L_DEBUG)
#endif
}

AboutGetRequest AboutService::Get() {
    AboutGetRequest agr(_cred, ABOUT_URL);
    return agr;
}

}
