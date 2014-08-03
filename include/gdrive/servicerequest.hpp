#ifndef __GDRIVE_SERVICEREQUEST_HPP__
#define __GDRIVE_SERVICEREQUEST_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/logging.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/filecontent.hpp"

#include <vector>
#include <set>

#define FILE_URL SERVICE_URI "/files"
#define ABOUT_URL SERVICE_URI "/about"

#define STRING_SET_ATTR(name) void set_##name(std::string name) { \
    _query[#name] = name;\
}

#define LONG_SET_ATTR(name) void set_##name(long name) { \
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
            _fields.insert(field);
        };
    protected: 
        GFile get_file();
        std::set<std::string> _fields;
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
        FileAttachedRequest(GFile* file, Credential* cred, std::string uri, RequestMethod method)
            :FieldRequest(cred, uri, method), _file(file) {}
    protected:
        void _json_encode_body();
        GFile* _file;
};

class FilePatchRequest : public FileAttachedRequest {
    CLASS_MAKE_LOGGER
    public:
        FilePatchRequest(GFile* file, Credential* cred, std::string uri)
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
        std::set<std::string> _parents;
};

class FileCopyRequest : public FileAttachedRequest {
    CLASS_MAKE_LOGGER
    public:
        FileCopyRequest(GFile* file, Credential* cred, std::string uri)
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


enum UploadType {
    UT_CREATE,
    UT_UPDATE
};

class FileUploadRequest: public FileAttachedRequest {
    CLASS_MAKE_LOGGER
    public:
        FileUploadRequest(FileContent* content, GFile* file, Credential* cred, std::string uri, bool resumable = false)
            :FileAttachedRequest(file, cred, uri, RM_POST), _content(content), _resumable(resumable), _type(UT_CREATE) {}

        GFile execute();
        BOOL_SET_ATTR(convert)
        BOOL_SET_ATTR(ocr)
        STRING_SET_ATTR(orcLanguag)
        BOOL_SET_ATTR(pinned) 
        STRING_SET_ATTR(timedTextLanguge)
        STRING_SET_ATTR(timedTextTrackName)
        BOOL_SET_ATTR(useContentAsIndexableText)
        void set_visibilty(std::string v) {
            if (v == "DEFAULT" || v == "PRIVATE")
                _query["visibility"] = v;
        }

    protected:
        std::string _generate_boundary() { return "======xxxxx=="; }
        int _resume();
        FileContent* _content;
        bool _resumable;
        UploadType _type;
};

typedef FileUploadRequest FileInsertRequest;

class FileUpdateRequest: public FileUploadRequest {
    CLASS_MAKE_LOGGER
    public:
        FileUpdateRequest(FileContent* content, GFile* file, Credential* cred, std::string uri, bool resumable = false)
            :FileUploadRequest(content, file, cred, uri, resumable)
        { 
            _type = UT_UPDATE;
            _method = RM_PUT;
        }

        void add_parent(std::string parent);
        void remove_parent(std::string parent);

    private:
        std::set<std::string> _parents;
};

class AboutGetRequest: public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        AboutGetRequest(Credential* cred, std::string uri)
            :CredentialHttpRequest(cred, uri, RM_GET) {}
        
        GAbout execute();
        BOOL_SET_ATTR(includeSubscribed)
        LONG_SET_ATTR(maxChangeIdCount)
        LONG_SET_ATTR(startChangeId)
};

}

#endif
