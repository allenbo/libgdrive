#include "gdrive/servicerequest.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;

namespace GDRIVE {

FieldRequest::FieldRequest(Credential* cred, std::string uri, RequestMethod method)
    :CredentialHttpRequest(cred, uri, method)
{
}

void FieldRequest::add_field(std::string field) {
    if (_query.find("fields") == _query.end()) {
        _query["fields"] = field;
    } else {
        _query["fields"] += "," + field;
    }
}

void FieldRequest::clear_fields() {
    if (_query.find("fields") == _query.end()) return;
    _query.erase("fields");
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



}
