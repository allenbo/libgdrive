#include "gdrive/servicerequest.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;

#define RESUMABLE_THRESHOLD 5 * 1024 * 1024
#define RESUMABLE_CHUNK_SIZE 256 * 1024

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
    JObject* tmp = _file->to_json();
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
    _header["Content-Length"] = VarString::itos(_body.size());
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
    _fields = _file->get_modified_fields();
    _json_encode_body();
    return get_file();
}

int FileUploadRequest::_resume() {
    clear();
    int cur_pos = 0;
    _read_hook = NULL;
    _read_context = NULL;
    _header["Content-Length"] = "0";
    _header["Content-Range"] = "bytes */" + VarString::itos(_content->get_length());
    FileAttachedRequest::request();
    if ( _resp.status() == 308) {
        std::string range = _resp.get_header("Range");
        cur_pos = atoi(VarString::split(range, "-")[1].c_str());
    } else {
        CLOG_ERROR("Unknown status from server %d while resuming, This is the error message %s\n", _resp.status(), _resp.content().c_str());
    }
    _read_hook = FileContent::resumable_read;
    _read_context = (void*)_content;
    return cur_pos;
}

GFile FileUploadRequest::execute() {
    int upload_type = -1;
    _fields = _file->get_modified_fields();
    if (_fields.size() == 0 ) {
        if ( _resumable == true || _content->get_length() >= RESUMABLE_THRESHOLD) {
            upload_type = 2;
            _query["uploadType"] = "resumable";
        } else {
            upload_type = 0;
            _query["uploadType"] = "media";
        }
    } else {
        if ( _resumable == true || _content->get_length() >= RESUMABLE_THRESHOLD) {
            upload_type = 2;
            _query["uploadType"] = "resumable";
        } else {
            upload_type = 1;
            _query["uploadType"] = "multipart";
        }
    }

    if (upload_type == 0) { // simple upload
        _read_hook = FileContent::read;
        _read_context = (void*)_content;
        _header["Content-Type"] = _content->mimetype();
        _header["Content-Length"] = VarString::itos(_content->get_length());
        FileAttachedRequest::request();
        if ((_type == UT_CREATE && _resp.status() != 200) || (_type == UT_UPDATE && _resp.status() != 201))
            CLOG_ERROR("Unknown status from server %d, This is the error message %s\n", _resp.status(), _resp.content().c_str());
    } else if (upload_type == 1) { // multipart upload
        _json_encode_body();
        std::string boundary = _generate_boundary();
        _header["Content-Type"] = "multipart/related; boundary=\"" + boundary + "\"";

        _body = "--" + boundary + "\n"
              + "Content-Type: application/json" + "\n\n"
              + _body + "\n"
              + "--" + boundary + "\n"
              + "Content-Type: " + _content->mimetype() + "\n\n"
              + _content->get_content() + "\n"
              + "--" + boundary + "--";
        _header["Content-Length"] = VarString::itos(_body.size());
        FileAttachedRequest::request();
        if ((_type == UT_CREATE && _resp.status() != 200) || (_type == UT_UPDATE && _resp.status() != 201))
            CLOG_ERROR("Unknown status from server %d, This is the error message %s\n", _resp.status(), _resp.content().c_str());

    } else {
        // Step 1 - Start a resumable session
        _header["X-Upload-Content-Type"] = _content->mimetype();
        _header["X-Upload-Content-Length"] = VarString::itos(_content->get_length());
        if (_fields.size() != 0) {
            _json_encode_body();
        }
        FileAttachedRequest::request();
        
        // Step 2 - Save the resumable session URI
        if (_resp.status() != 200) 
            CLOG_ERROR("Unknown status from server %d after step 1, This is the error message %s\n", _resp.status(), _resp.content().c_str());

        std::string location = _resp.get_header("Location");

        // Prepare for step 3
        set_uri(location);
        _method = RM_PUT;

        // Step 3 - Upload the file
        int file_length = _content->get_length();
        if (_content->get_length() > RESUMABLE_CHUNK_SIZE) { // Uploading the file in chunks
            int cur_pos = 0;
            while (true) {
                clear();
                int cur_length = file_length - cur_pos > RESUMABLE_CHUNK_SIZE ? RESUMABLE_CHUNK_SIZE : file_length - cur_pos;
                _header["Content-Length"] = VarString::itos(cur_length);
                _header["Content-Type"] = _content->mimetype();
                _header["Content-Range"] = "bytes " + VarString::itos(cur_pos) + "-" + VarString::itos(cur_pos + cur_length -1 ) + "/" + VarString::itos(file_length);
                _content->set_resumable_start_pos(cur_pos);
                _content->set_resumable_length(cur_length);
                _read_hook = FileContent::resumable_read;
                _read_context = (void*)_content;
                CLOG_DEBUG("Sending out from %d - %d/%d\n", cur_pos, cur_pos + cur_length - 1, file_length);
                FileAttachedRequest::request();
                
                if (_resp.status() == 308) {
                    CLOG_DEBUG("Resumabled\n");
                    std::string range = _resp.get_header("Range");
                    cur_pos = atoi(VarString::split(range, "-")[1].c_str()) + 1;
                } else if (_resp.status() == 200 || _resp.status() == 201) {
                    break;
                } else if (_resp.status() >= 500) {
                    // resume an interrupted upload
                    cur_pos = _resume();
                } else {
                    CLOG_ERROR("Unknown status from server %d after step 3, This is the error message %s\n", _resp.status(), _resp.content().c_str());
                }
            }
        } else { // Uploading the file completely in one request
            int cur_pos;
            while(true) {
                clear();
                _header["Content-Length"] = VarString::itos(_content->get_length());
                _header["Content-Type"] = _content->mimetype();
                _content->set_resumable_start_pos(cur_pos);
                _content->set_resumable_length(file_length - cur_pos);
                _read_hook = FileContent::resumable_read;
                _read_context = (void*)_content;
                FileAttachedRequest::request();
                _read_hook = NULL;
                _read_context = NULL;
                if (_resp.status() == 200 || _resp.status() == 201) {
                    break;
                } else if (_resp.status() >= 500 ){
                    // resume an interrupted upload
                    cur_pos = _resume();
                } else {
                    CLOG_ERROR("Unknown status from server %d after step 3, This is the error message %s\n", _resp.status(), _resp.content().c_str());
                }
            }
        }
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

void FileUpdateRequest::add_parent(std::string parent) {
    _parents.insert(parent);
    _query["addParents"] = VarString::join(_parents,",");
}

void FileUpdateRequest::remove_parent(std::string parent) {
    std::set<std::string>::iterator iter = find(_parents.begin(), _parents.end(), parent);
    if (iter != _parents.end()) {
        _parents.erase(iter);
        _query["addParents"] = VarString::join(_parents,",");
    }
}


GAbout AboutGetRequest::execute() {
    CredentialHttpRequest::request();
    if (_resp.status() != 200)
        CLOG_ERROR("Unknown status from server %d, This is the error message %s\n", _resp.status(), _resp.content().c_str());

    PError error;
    JObject* obj = (JObject*)loads(_resp.content(), error);
    GAbout about;
    if (obj != NULL) {
        about.from_json(obj);
        delete obj;
    }
    return about;
}

GChange ChangeGetRequest::execute() {
    CredentialHttpRequest::request();
    if (_resp.status() != 200)
        CLOG_ERROR("Unknown status from server %d, This is the error message %s\n", _resp.status(), _resp.content().c_str());

    PError error;
    JObject* obj = (JObject*)loads(_resp.content(), error);
    GChange change;
    if (obj != NULL) {
        change.from_json(obj);
        delete obj;
    }
    return change;
}

}
