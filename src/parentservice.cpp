#include "gdrive/parentservice.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

ParentService ParentService::_single_instance;

ParentService::ParentService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("FileService", L_DEBUG)
#endif
}

}
