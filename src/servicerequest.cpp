#include "gdrive/servicerequest.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;

#define RESUMABLE_THRESHOLD 5 * 1024 * 1024

namespace GDRIVE {

GFile FieldRequest::get_file() {
    CredentialHttpRequest::request();
    if (_resp.status() != 200)
        CLOG_ERROR("Unknown status from server %d, This is the error message %s\n", _resp.status(), _resp.content().c_str());

    PError error;
    JObject* obj = (JObject*)loads(_resp.content(), error);
    GFile file;
    if (obj != NULL) {
        file.from_json(obj);
        delete obj;
    }
    return file;
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
                    CLOG_DEBUG("Get file %s\n", file.get_title().c_str());
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

GFile FileGetRequest::execute() {
    return FieldRequest::get_file();
}

GFile FileTrashRequest::execute() {
    return FieldRequest::get_file();
}

bool FileDeleteRequest::execute() {
    CredentialHttpRequest::request();
    if (_resp.status() == 204) {
        return true;
    } else {
        CLOG_WARN("%d: %s\n", _resp.status(), _resp.content().c_str());
        return false;
    }
}

void FileAttachedRequest::_json_encode_body() {
    JObject* tmp = _file.to_json();
    JObject* rst_obj = new JObject();
    //for(int i = 0; i < _fields.size(); i ++ ) {
    for(std::set<std::string>::iterator iter = _fields.begin();
            iter != _fields.end(); iter ++) {
        std::string field = *iter;
        if (tmp->contain(field)) {
            JValue* v = tmp->pop(field);
            rst_obj->put(field, v);
        }
    }
    char* buf;
    dumps(rst_obj, &buf);
    delete tmp;
    delete rst_obj;
    _body = std::string(buf);
    free(buf);

    _header["Content-Type"] = "application/json";
}


void FilePatchRequest::add_parent(std::string parent) {
    _parents.insert(parent);
    _query["addParents"] = VarString::join(_parents,",");
}

void FilePatchRequest::remove_parent(std::string parent) {
    std::set<std::string>::iterator iter = find(_parents.begin(), _parents.end(), parent);
    if (iter != _parents.end()) {
        _parents.erase(iter);
        _query["addParents"] = VarString::join(_parents,",");
    }
}

GFile FilePatchRequest::execute() {
    _json_encode_body();
    return get_file();
}

GFile FileCopyRequest::execute() {
    if (_query.find("fields") != _query.end()) {
        _query.erase("fields");
    }
    _fields = _file.get_modified_fields();
    _json_encode_body();
    return get_file();
}

GFile FileInsertRequest::execute() {
    int upload_type = -1;
    _fields = _file.get_modified_fields();
    if (_fields.size() == 0 ) {
        if ( _resumable == true || _content.get_length() >= RESUMABLE_THRESHOLD) {
            upload_type = 2;
            _query["uploadType"] = "resumable";
        } else {
            upload_type = 0;
            _query["uploadType"] = "media";
        }
    } else {
        if ( _resumable == true || _content.get_length() >= RESUMABLE_THRESHOLD) {
            upload_type = 2;
            _query["uploadType"] = "resumable";
        } else {
            upload_type = 1;
            _query["uploadType"] = "multipart";
        }
    }

    if (upload_type == 0) { // simple upload
        _read_hook = FileContent::read;
        _read_context = (void*)&_content;
        _header["Content-Type"] = _content.mimetype();
        _header["Content-Length"] = VarString::itos(_content.get_length());
        FileAttachedRequest::request();
        if (_resp.status() != 200)
            CLOG_ERROR("Unknown status from server %d, This is the error message %s\n", _resp.status(), _resp.content().c_str());
    } else if (upload_type == 1) { // multipart upload
        _json_encode_body();
        std::string boundary = _generate_boundary();
        _header["Content-Type"] = "multipart/related; boundary=\"" + boundary + "\"";

        _body = "--" + boundary + "\r\n"
              + "Content-Type: application/json; charset=UTF-8" + "\r\n"
              + _body + "\r\n"
              + "--" + boundary + "\r\n"
              + "Content-Type: " + _content.mimetype() + "\r\n"
              + _content.get_content() + "\r\n"
              + "--" + boundary + "--";
        _header["Content-Length"] = _body.size();
    } else {
    }

    PError error;
    JObject* obj = (JObject*)loads(_resp.content(), error);
    GFile file;
    if (obj != NULL) {
        file.from_json(obj);
        delete obj;
    }
    return file;
}

}
