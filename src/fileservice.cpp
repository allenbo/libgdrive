#include "gdrive/fileservice.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;

namespace GDRIVE {

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

FileGetRequest FileService::Get(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id);
    FileGetRequest fgr(_cred, vs.toString(), RM_GET);
    return fgr;
}

FileTrashRequest FileService::Trash(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id).append("/trash");
    FileTrashRequest request(_cred, vs.toString(), RM_POST);
    return request;
}

FileUntrashRequest FileService::Untrash(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id).append("/untrash");
    FileUntrashRequest request(_cred, vs.toString(), RM_POST);
    return request;
}

FileDeleteRequest FileService::Delete(std::string id) {
    VarString vs;
    vs.append(FILE_URL).append('/').append(id);
    FileDeleteRequest request(_cred, vs.toString(), RM_DELETE);
    return request;
}

/*
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
