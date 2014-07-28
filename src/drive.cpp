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
                    CLOG_DEBUG("Get file %s\n", file.title.c_str());
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

GFile FileService::Get(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id);
    Request request(vs.toString(), RM_GET);
    Response resp = _cred.request(request);
    if (resp.status() == 200) {
        PError error;
        JObject* obj = (JObject*)loads(resp.content(), error);
        GFile file;
        if (obj != NULL) {
            file.from_json(obj);
            delete obj;
        }
        return file;
    } else if (resp.status() == 404) {
        CLOG_ERROR("There is no such file found: %s\n", id.c_str()); 
    } else {
        CLOG_ERROR("Unknown status from server %d, This is the error message %s\n", resp.status(), resp.content().c_str());
    }
}

GFile FileService::Trash(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id).append("/trash");
    Request request(vs.toString(), RM_POST);
    Response resp = _cred.request(request);
    if (resp.status() == 200) {
        PError error;
        JObject* obj = (JObject*)loads(resp.content(), error);
        GFile file;
        if (obj != NULL) {
            file.from_json(obj);
            delete obj;
        }
        return file;
    } else {
        CLOG_ERROR("Not sucessful, server returns %d: %s\n", resp.status(), resp.content().c_str());
    }
}

GFile FileService::Untrash(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id).append("/untrash");
    Request request(vs.toString(), RM_POST);
    Response resp = _cred.request(request);
    if (resp.status() == 200) {
        PError error;
        JObject* obj = (JObject*)loads(resp.content(), error);
        GFile file;
        if (obj != NULL) {
            file.from_json(obj);
            delete obj;
        }
        return file;
    } else {
        CLOG_ERROR("Not sucessful, server returns %d: %s\n", resp.status(), resp.content().c_str());
    }
}

bool FileService::Delete(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id);
    Request request(vs.toString(), RM_DELETE);
    Response resp = _cred.request(request);
    if ( resp.status() == 204) {
        return true;
    } else {
        CLOG_WARN("%d: %s\n", resp.status(), resp.content().c_str());
        return false;
    }
}

bool FileService::EmptyTrash() {
    VarString vs;
    vs.append(FILE_URL).append("/trash");
    Request request(vs.toString(), RM_DELETE);
    Response resp = _cred.request(request);
    if ( resp.status() == 204) {
        return true;
    } else {
        CLOG_WARN("%d: %s\n", resp.status(), resp.content().c_str());
        return false;
    }

}

GFile FileService::Touch(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id).append("/touch");
    Request request(vs.toString(), RM_POST);
    Response resp = _cred.request(request);
    if (resp.status() == 200) {
        PError error;
        JObject* obj = (JObject*)loads(resp.content(), error);
        GFile file;
        if (obj != NULL) {
            file.from_json(obj);
            delete obj;
        }
        return file;
    } else {
        CLOG_ERROR("Not sucessful, server returns %d: %s\n", resp.status(), resp.content().c_str());
    }
}

}
