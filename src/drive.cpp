#include "gdrive/drive.hpp"
#include "jconer/json.hpp"
using namespace JCONER;
namespace GDRIVE {

DriveService::DriveService(Credential cred)
    :_cred(cred)
{
}

FileService::FileService(Credential cred)
    :DriveService(cred)
{
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("FileService", L_DEBUG)
#endif
}

std::vector<GFile> FileService::List() {
    std::vector<GFile> files;
    std::string next_link = "";
    RequestBody body;
    body["maxRequest"] = "1000";
    Request request(FILE_URL, RM_GET);
    request.add_body(body);

    while(true) {
        Response resp = _cred.request(request);
        PError error;
        JObject* value = (JObject*)loads(resp.content(), error);
        if (value != NULL) {
            if (value->contain("items")) {
                JArray* items = (JArray*)value->get("items");
                for(int i = 0; i < items->size(); i ++ ) {
                    JObject* item = (JObject*)items->get(i);
                    GFile file;
                    file.from_json(item);
                    files.push_back(file);
                    CLOG_DEBUG("Get %d file\n", files.size());
                }
            }
            if (value->contain("nextLink")) {
                next_link = ((JString*)value->get("nextLink"))->getValue();
                request = Request(next_link, RM_GET);
                delete value;
            } else {
                delete value;
                break;
            }
        }
    }
    return files;
}
}
