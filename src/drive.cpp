#include "gdrive/drive.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;
namespace GDRIVE {

Drive::Drive(Credential *cred)
    :_cred(cred)
{
}

FileListRequest::FileListRequest(Credential* cred, std::string uri, RequestMethod method)
    :CredentialHttpRequest(cred, uri, method)
{
}

void FileListRequest::set_corpus(std::string corpus) {
    if (corpus == "DEFAULT" or corpus == "DOMAIN") {
        _query["corpus"] = corpus;
    } else {
        CLOG_WARN("Wrong corpus parameter[%s], using DEFAULT\n", corpus.c_str());
    }
}

void FileListRequest::set_max_results(int max_results) {
    if (max_results >= 0) {
        _query["maxResults"] = VarString::itos(max_results);
    } else {
        CLOG_WARN("Wrong maxResults parameter[%d], using 100\n", max_results);
    }
}

void FileListRequest::set_page_token(std::string page_token) {
    _query["pageToken"] = page_token;
}

void FileListRequest::set_q(std::string q) {
    _query["q"] = q;
}

std::vector<GFile> FileListRequest::execute() {
    std::vector<GFile> files;
    std::string next_link = "";

    while(true) {
        CredentialHttpRequest::request();
        PError error;
        JObject* value = (JObject*)loads(_resp.content(), error);
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
                set_uri(next_link);
                _query.clear();
                delete value;
            } else {
                delete value;
                break;
            }
        }
    }
    return files;
}

FileService& Drive::files() {
    return FileService::get_instance(_cred);
}

FileService FileService::_single_instance;

FileService::FileService()
{
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("FileService", L_DEBUG)
#endif
}

FileListRequest FileService::List() {
    FileListRequest flr(_cred, FILE_URL, RM_GET);
    return flr;
}
/*
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

FileService::PatchRequest Patch(std::string file_id, GFile file) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(file_id);
    FileService::PatchRequest pr(vs.toString(), file_id, file);
    return pr;
}

FileService::PatchRequest::PatchRequest(std::string uri, std::string file_id, GFile file)
    :Request(uri, RM_PATCH), _file_id(file_id), _file(file) 
{
}

std::string FileService::PatchRequest::_jsonencode_body() {
    JObject* tmp = _file.to_json();
    JObject* rst_obj = new JObject();
    for(int i = 0; i < _fields.size(); i ++ ) {
        std::string field = _fields[i];
        if (tmp->contain(field)) {
            rst_obj->put(field, tmp->get(field)->deepcopy());
        }
    }
    char* buf;
    dumps(rst_obj, &buf);
    delete tmp;
    delete rst_obj;
    std::string rst(buf);
    return rst;
}

GFile FileService::PatchRequest::execute() {
    _cred.request(*this);
    GFile file = _file;
    if (_resp.status() == 200) {
        PError error;
        JObject* obj = (JObject*)loads(_resp.content(), error);
        if (obj != NULL) {
            file.from_json(obj);
            delete obj;
        }
        return file;
    } else {
        CLOG_ERROR("Unknow response: %d %s\n", _resp.status(), _resp.content().c_str());
    }
}
*/

}
