#include "gdrive/servicerequest.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;

#define RESUMABLE_THRESHOLD 5 * 1024 * 1024
#define RESUMABLE_CHUNK_SIZE 256 * 1024

namespace GDRIVE {

GoogleJsonResponseException make_json_exception(std::string content) {
    GError gerror;
    PError perror;
    JObject* obj = (JObject*)loads(content, perror);
    if (obj != NULL) {
        gerror.from_json(obj);
        delete obj;
    }
    return GoogleJsonResponseException(gerror);
}

void DeleteRequest::execute() {
    CredentialHttpRequest::request();
    if (_resp.status() != 204) {
        GoogleJsonResponseException exc = make_json_exception(_resp.content());
        throw exc;
    }   
}

void FileListRequest::set_corpus(std::string corpus) {
    if (corpus == "DEFAULT" or corpus == "DOMAIN") {
        _query["corpus"] = corpus;
    } else {
        CLOG_WARN("Wrong corpus parameter[%s], using DEFAULT\n", corpus.c_str());
    }
}

void FileListRequest::set_maxResults(int max_results) {
    if (max_results >= 0) {
        _query["maxResults"] = VarString::itos(max_results);
    } else {
        CLOG_WARN("Wrong maxResults parameter[%d], using 100\n", max_results);
    }
}

int FileUploadRequest::_resume() {
    clear();
    int cur_pos = 0;
    _read_hook = NULL;
    _read_context = NULL;
    _header["Content-Length"] = "0";
    _header["Content-Range"] = "bytes */" + VarString::itos(_content->get_length());
    request();
    if ( _resp.status() == 308) {
        std::string range = _resp.get_header("Range");
        cur_pos = atoi(VarString::split(range, "-")[1].c_str());
    } else {
        GoogleJsonResponseException exc = make_json_exception(_resp.content());
        throw exc;
    }
    _read_hook = FileContent::resumable_read;
    _read_context = (void*)_content;
    return cur_pos;
}

GFile FileUploadRequest::execute() {
    int upload_type = -1;
    std::set<std::string> fields = _resource->get_modified_fields();
    if (fields.size() == 0 ) {
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
        request();
        if ((_type == UT_CREATE && _resp.status() != 200) || (_type == UT_UPDATE && _resp.status() != 201)) {
            GoogleJsonResponseException exc = make_json_exception(_resp.content());
            throw exc;
        }
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
        request();
        if ((_type == UT_CREATE && _resp.status() != 200) || (_type == UT_UPDATE && _resp.status() != 201)) {
            GoogleJsonResponseException exc = make_json_exception(_resp.content());
            throw exc;
        }

    } else {
        // Step 1 - Start a resumable session
        _header["X-Upload-Content-Type"] = _content->mimetype();
        _header["X-Upload-Content-Length"] = VarString::itos(_content->get_length());
        if (fields.size() != 0) {
            _json_encode_body();
        }
        request();
        
        // Step 2 - Save the resumable session URI
        if (_resp.status() != 200)  {
            GoogleJsonResponseException exc = make_json_exception(_resp.content());
            throw exc;
        }

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
                request();
                
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
                    GoogleJsonResponseException exc = make_json_exception(_resp.content());
                    throw exc;
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
                request();
                _read_hook = NULL;
                _read_context = NULL;
                if (_resp.status() == 200 || _resp.status() == 201) {
                    break;
                } else if (_resp.status() >= 500 ){
                    // resume an interrupted upload
                    cur_pos = _resume();
                } else {
                    GoogleJsonResponseException exc = make_json_exception(_resp.content());
                    throw exc;
                }
            }
        }
    }
    GFile _1 = *_resource;
    this->get_resource(_1);
    return _1;
}

}
