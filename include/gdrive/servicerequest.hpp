#ifndef __GDRIVE_SERVICEREQUEST_HPP__
#define __GDRIVE_SERVICEREQUEST_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/filecontent.hpp"
#include "common/all.hpp"

#include <vector>
#include <set>

#define FILES_URL SERVICE_URI "/files"
#define ABOUT_URL SERVICE_URI "/about"
#define CHANGES_URL SERVICE_URI "/changes"

#define STRING_SET_ATTR(name) void set_##name(std::string name) { \
    _query[#name] = name;\
}

#define LONG_SET_ATTR(name) void set_##name(long name) { \
    _query[#name] = COMMON::VarString::itos(name);\
}

#define BOOL_SET_ATTR(name) void set_##name(bool flag) { \
    if (flag) _query[#name] = "true";\
    else _query[#name] = "false"; \
}

namespace GDRIVE {

template<class ResType, RequestMethod method>
class ResourceRequest : public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        ResourceRequest(Credential* cred, std::string uri)
            :CredentialHttpRequest(cred, uri, method) {}
        ResType execute();
};

template<class ResType, RequestMethod method>
ResType ResourceRequest<ResType, method>::execute() {
    CredentialHttpRequest::request();
    if (_resp.status() != 200)
        CLOG_ERROR("Unknown status from server %d, This is the error message %s\n", _resp.status(), _resp.content().c_str());

    PError error;
    JObject* obj = (JObject*)loads(_resp.content(), error);
    ResType _1;
    if (obj != NULL) {
        _1.from_json(obj);
        delete obj;
    }
    return _1;
}

class DeleteRequest : public CredentialHttpRequest {
    CLASS_MAKE_LOGGER
    public:
        DeleteRequest(Credential* cred, std::string uri)
            :CredentialHttpRequest(cred, uri, RM_DELETE) {}
        bool execute();
};

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

class FileListRequest: public ResourceRequest<GFileList, RM_GET> {
    CLASS_MAKE_LOGGER
    public:
        FileListRequest(Credential* cred, std::string uri)
            :ResourceRequest<GFileList, RM_GET>(cred, uri) {}
        STRING_SET_ATTR(pageToken)
        STRING_SET_ATTR(q)
        void set_corpus(std::string corpus);
        void set_maxResults(int max_results);
};

class FileGetRequest: public ResourceRequest<GFile, RM_GET> {
    CLASS_MAKE_LOGGER
    public:
        FileGetRequest(Credential* cred, std::string uri)
            :ResourceRequest<GFile, RM_GET>(cred, uri) {}
        BOOL_SET_ATTR(updateViewedDate)
};

typedef ResourceRequest<GFile, RM_POST> FileTrashRequest;
typedef FileTrashRequest FileUntrashRequest;
typedef DeleteRequest FileDeleteRequest;
typedef FileDeleteRequest FileEmptyTrashRequest;
typedef ResourceRequest<GFile, RM_POST> FileTouchRequest;

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

class AboutGetRequest: public ResourceRequest<GAbout, RM_GET> {
    CLASS_MAKE_LOGGER
    public:
        AboutGetRequest(Credential* cred, std::string uri)
            :ResourceRequest<GAbout, RM_GET>(cred, uri) {}
        
        BOOL_SET_ATTR(includeSubscribed)
        LONG_SET_ATTR(maxChangeIdCount)
        LONG_SET_ATTR(startChangeId)
};

typedef ResourceRequest<GChange, RM_GET> ChangeGetRequest;

class ChangeListRequest: public ResourceRequest<GChangeList, RM_GET> {
    CLASS_MAKE_LOGGER
    public:
        ChangeListRequest(Credential* cred, std::string uri)
            :ResourceRequest<GChangeList, RM_GET>(cred, uri) {}

        BOOL_SET_ATTR(includeDeleted)
        BOOL_SET_ATTR(includeSubscribed)
        STRING_SET_ATTR(pageToken)
        LONG_SET_ATTR(maxResults)
        LONG_SET_ATTR(startChangeId)
};


class ChildrenListRequest: public ResourceRequest<GChildrenList, RM_GET> {
    CLASS_MAKE_LOGGER
    public:
        ChildrenListRequest(Credential* cred, std::string uri)
            :ResourceRequest<GChildrenList, RM_GET>(cred, uri) {}

        LONG_SET_ATTR(maxResults)
        STRING_SET_ATTR(pageToken)
        STRING_SET_ATTR(q)
};

typedef ResourceRequest<GChildren, RM_GET> ChildrenGetRequest;

class ChildrenInsertRequest: public ResourceRequest<GChildren, RM_POST> {
    CLASS_MAKE_LOGGER
    public:
        ChildrenInsertRequest(GChildren* child, Credential* cred, std::string uri)
            :ResourceRequest<GChildren, RM_POST>(cred, uri), _child(child) {}
        GChildren execute();

    private:
        void _json_encode_body();
        GChildren * _child;
};

typedef DeleteRequest ChildrenDeleteRequest;
typedef ResourceRequest<GParentList, RM_GET> ParentListRequest;
typedef ResourceRequest<GParent, RM_GET> ParentGetRequest;

class ParentInsertRequest: public ResourceRequest<GParent, RM_POST> {
    CLASS_MAKE_LOGGER
    public:
        ParentInsertRequest(GParent* parent, Credential* cred, std::string uri)
            :ResourceRequest<GParent, RM_POST>(cred, uri), _parent(parent) {}
        GParent execute();

    private:
        void _json_encode_body();
        GParent * _parent;
};

typedef DeleteRequest ParentDeleteRequest;
typedef ResourceRequest<GPermissionList, RM_GET> PermissionListRequest;
typedef ResourceRequest<GPermission, RM_GET> PermissionGetRequest;

class PermissionInsertRequest: public ResourceRequest<GPermission, RM_POST> {
    CLASS_MAKE_LOGGER
    public:
        PermissionInsertRequest(GPermission* permission, Credential* cred, std::string uri)
            :ResourceRequest<GPermission, RM_POST>(cred, uri), _permission(permission) {}
        GPermission execute();
        STRING_SET_ATTR(emailMessage)
        BOOL_SET_ATTR(sendNotificationEmails)

    private:
        void _json_encode_body();
        GPermission * _permission;
        std::set<std::string> _fields;
};

typedef DeleteRequest PermissionDeleteRequest;

}

#endif
