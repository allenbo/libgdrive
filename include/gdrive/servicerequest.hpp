#ifndef __GDRIVE_SERVICEREQUEST_HPP__
#define __GDRIVE_SERVICEREQUEST_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/logging.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"

#include <vector>

#define FILE_URL SERVICE_URI "/files"

#define STRING_SET_ATTR(name) void set_##name(std::string name) { \
    _query[#name] = name;\
}

#define BOOL_SET_ATTR(name) void set_##name(bool flag) { \
    if (flag) _query[#name] = "true";\
    else _query[#name] = "false"; \
}

namespace GDRIVE {

class FieldRequest: public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        FieldRequest(Credential* cred, std::string uri, RequestMethod method)
            :CredentialHttpRequest(cred, uri, method) {}
        inline void clear_fields() {
            if (_query.find("fields") == _query.end()) return;
            _query.erase("fields");
            _fields.clear();
        }

        inline void add_field(std::string field) {
            if (_query.find("fields") == _query.end()) {
                _query["fields"] = field;
            } else {
                _query["fields"] += "," + field;
            }
            _fields.push_back(field);
        };
    protected: 
        GFile get_file();
        std::vector<std::string> _fields;
};

class FileListRequest: public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        FileListRequest(Credential* cred, std::string uri)
            :CredentialHttpRequest(cred, uri, RM_GET) {}
        std::vector<GFile> execute(); 
        STRING_SET_ATTR(pageToken)
        STRING_SET_ATTR(q)
        void set_corpus(std::string corpus);
        void set_max_results(int max_results);
};

class FileGetRequest: public FieldRequest {
    CLASS_MAKE_LOGGER
    public:
        FileGetRequest(Credential* cred, std::string uri)
            :FieldRequest(cred, uri, RM_GET) {}
        GFile execute();
        BOOL_SET_ATTR(updateViewedDate)
};

class FileTrashRequest: public FieldRequest {
    CLASS_MAKE_LOGGER
    public:
        FileTrashRequest(Credential* cred, std::string uri)
            :FieldRequest(cred, uri, RM_POST) {}
        GFile execute();
};

typedef FileTrashRequest FileUntrashRequest;

class FileDeleteRequest: public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        FileDeleteRequest(Credential* cred, std::string uri)
            :CredentialHttpRequest(cred, uri, RM_DELETE) {}
        bool execute();
};

typedef FileDeleteRequest FileEmptyTrashRequest;


class FileTouchRequest: public FieldRequest {
    CLASS_MAKE_LOGGER
    public:
        FileTouchRequest(Credential* cred, std::string uri)
            :FieldRequest(cred, uri, RM_POST) {}
        GFile execute() { return get_file(); }
};

class FileAttachedRequest : public FieldRequest {
    CLASS_MAKE_LOGGER
    public:
        FileAttachedRequest(GFile file, Credential* cred, std::string uri, RequestMethod method)
            :FieldRequest(cred, uri, method), _file(file) {}
    protected:
        void _json_encode_body();
        GFile _file;
};

class FilePatchRequest : public FileAttachedRequest {
    CLASS_MAKE_LOGGER
    public:
        FilePatchRequest(GFile file, Credential* cred, std::string uri)
            :FileAttachedRequest(file, cred, uri, RM_PATCH) {}

        GFile execute();
        void add_parent(std::string parent);
        void remove_parent(std::string parent);

        BOOL_SET_ATTR(convert)
        BOOL_SET_ATTR(newRevision)
        BOOL_SET_ATTR(ocr)
        STRING_SET_ATTR(orcLanguag)
        BOOL_SET_ATTR(pinned) 
        BOOL_SET_ATTR(setModifiedDate)
        STRING_SET_ATTR(timedTextLanguge)
        STRING_SET_ATTR(timedTextTrackName)
        BOOL_SET_ATTR(updateViewedDate)
        BOOL_SET_ATTR(useContentAsIndexableText)
    private:
        std::vector<std::string> _parents;
};

class FileCopyRequest : public FileAttachedRequest {
    CLASS_MAKE_LOGGER
    public:
        FileCopyRequest(GFile file, Credential* cred, std::string uri)
            :FileAttachedRequest(file, cred, uri, RM_POST) {}

        GFile execute();
        BOOL_SET_ATTR(convert)
        BOOL_SET_ATTR(ocr)
        STRING_SET_ATTR(ocrLanguage)
        BOOL_SET_ATTR(pinned)
        STRING_SET_ATTR(timedTextLanguage)
        STRING_SET_ATTR(timedTextTrackName)
        void set_visibilty(std::string v) {
            if (v == "DEFAULT" || v == "PRIVATE")
                _query["visibility"] = v;
        }
};

}

#endif
