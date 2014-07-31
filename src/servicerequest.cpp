#include "gdrive/servicerequest.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;

namespace GDRIVE {

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


FileGetRequest::FileGetRequest(Credential* cred, std::string uri, RequestMethod method)
    :FieldRequest(cred, uri, method)
{
}

void FileGetRequest::set_update_viewed_date(bool flag) {
    if(flag)
        _query["updateViewedDate"] = "true";
    else
        _query["updateViewedDate"] = "false";
}

GFile FileGetRequest::execute() {
    FieldRequest::request();
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

}
