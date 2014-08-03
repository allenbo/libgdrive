#include "gdrive/drive.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;
namespace GDRIVE {

Drive::Drive(Credential *cred)
    :_cred(cred)
{
}

FileService& Drive::files() {
    return FileService::get_instance(_cred);
}

AboutService& Drive::about() {
    return AboutService::get_instance(_cred);
}

ChangeService& Drive::changes() {
    return ChangeService::get_instance(_cred);
}

}
