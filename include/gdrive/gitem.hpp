#ifndef __GDRIVE_GFILE_HPP__
#define __GDRIVE_GFILE_HPP__

#include <string>
#include <time.h>
#include <set>
#include <map>
#include <exception>

#include "jconer/json.hpp"

#define GETTER(type, name) type get_##name() { return name;}

#define SETTER(type, name) void set_##name(type v) {\
    name = v; \
    _fields.insert(#name); \
}

#define READONLY(type, name) \
    private: \
        type name;\
    public: \
        GETTER(type, name)

#define WRITABLE(type, name) \
    private: \
        type name; \
    public: \
        SETTER(type, name) \
        GETTER(type, name)


using namespace JCONER;

namespace GDRIVE {
// File representation

class GFileLabel {
public:
    GFileLabel();
    bool starred;
    bool hidden;
    bool trashed;
    bool restricted;
    bool viewed;
    void from_json(JObject* obj);
    JObject* to_json();
};

class GUser {
public:
    GUser();
    std::string displayName;
    std::string picture_url;
    bool isAuthenticatedUser;
    std::string permissionId;
    void from_json(JObject* obj);
    JObject* to_json();
};

class GParent {
public:
    GParent();
    WRITABLE(std::string, id)
    READONLY(std::string, selfLink)
    READONLY(std::string, parentLink)
    READONLY(bool, isRoot)
    void from_json(JObject* obj);
    JObject* to_json();

    std::set<std::string> get_modified_fields() { return _fields;}
    void clear() { _fields.clear();}

private:
    std::set<std::string> _fields;
};


class GParentList {
public:
    GParentList();
    void from_json(JObject* obj);
    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::vector<GParent>, items)
};

class GProperty {
public:
    GProperty();
    std::string etag;
    std::string selfLink;
    std::string key;
    std::string visibility;
    std::string value;
    void from_json(JObject* obj);
    JObject* to_json();
};

class GPermission {
public:
    GPermission();

    std::set<std::string> get_modified_fields() { return _fields;}
    void clear() { _fields.clear();}

    READONLY(std::string, etag)
    WRITABLE(std::string, id)
    READONLY(std::string, selfLink)
    READONLY(std::string, name)
    READONLY(std::string, emailAddress)
    READONLY(std::string, domain)
    WRITABLE(std::string, role)
    WRITABLE(std::vector<std::string>, additionalRoles)
    WRITABLE(std::string, type)
    WRITABLE(std::string, value)
    READONLY(std::string, authKey)
    READONLY(bool, withLink)
    READONLY(std::string, photoLink)

    void from_json(JObject* obj);
    JObject* to_json();
private:
    std::set<std::string> _fields;
};

class GPermissionId {
public:
    GPermissionId() {
        id = "";
    }
    void from_json(JObject *);

    READONLY(std::string, id);
};

class GPermissionList {
public:
    GPermissionList();
    void from_json(JObject* obj);

    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::vector<GPermission>, items)
};

class GImageMediaMetaData {
public:
    GImageMediaMetaData();
    int width;
    int height;
    int rotation;
    class Location {
    public:
        double latitude;
        double longitude;
        double altitude;
        void from_json(JObject* obj);
        JObject* to_json();
    } location;
    std::string date;
    std::string cameraMaker;
    std::string cameraModel;
    double exposureTime;
    double aperture;
    bool flashUsed;
    double focalLength;
    int isoSpeed;
    std::string meteringMode;
    std::string sensor;
    std::string exposureMode;
    std::string colorSpace;
    std::string whiteBalance;
    double exposureBias;
    double maxApertureValue;
    int subjectDistance;
    std::string lens;
    void from_json(JObject* obj);
    JObject* to_json();
};

typedef std::map<std::string, std::string> GExportLink;
typedef std::map<std::string, std::string> Links;
typedef std::map<std::string, std::string> string_map;

class GFile {
public:
    GFile();
    void from_json(JObject* obj);
    JObject* to_json();
    std::set<std::string> get_modified_fields() { return _fields;}
    void clear() { _fields.clear();}
    

    READONLY(std::string, id)
    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::string, webContentLink)
    READONLY(std::string, alternateLink)
    READONLY(std::string, embedLink)
    READONLY(Links, openWithLinks)
    READONLY(std::string, defaultOpenWithLink)
    READONLY(std::string, iconLink)
    READONLY(std::string, thumbnailLink)
    //thumbnail
    WRITABLE(std::string, title)
    WRITABLE(std::string, mimeType)
    WRITABLE(std::string, description)
    WRITABLE(GFileLabel, labels)
    READONLY(struct tm, createdDate)
    WRITABLE(struct tm, modifiedDate)
    READONLY(struct tm, modifiedByMeDate)
    READONLY(struct tm, lastViewedByMeDate)
    READONLY(struct tm, sharedWithMeDate)
    READONLY(std::string, version)
    READONLY(GUser, sharingUser)
    WRITABLE(std::vector<GParent>, parents)
    READONLY(std::string, downloadUrl)
    READONLY(GExportLink, exportLinks)
    WRITABLE(std::string, indexableText)
    READONLY(GPermission, userPermission)
    READONLY(std::vector<GPermission>, permissions)
    READONLY(std::string, originalFilename)
    READONLY(std::string, fileExtension)
    READONLY(std::string, md5Checksum)
    READONLY(long, fileSize)
    READONLY(int, quotaBytesUsed)
    READONLY(std::vector<std::string>, ownerNames)
    READONLY(std::vector<GUser>, owners)
    READONLY(std::string, lastModifyingUserName)
    READONLY(GUser, lastModifyingUser)
    READONLY(bool, editable)
    READONLY(bool, copyable)
    READONLY(bool, shared)
    READONLY(bool, explicitlyTrashed)
    READONLY(bool, appDataContents)
    READONLY(std::string, headRevisionId)
    READONLY(std::vector<GProperty>, properties)
    READONLY(GImageMediaMetaData, imageMediaMetadata)
    WRITABLE(bool, writersCanShare)
private:
    std::set<std::string> _fields;
};


class GFileList {
public:
    GFileList();
    void from_json(JObject* obj);

    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::string, nextPageToken)
    READONLY(std::string, nextLink)
    READONLY(std::vector<GFile>, items)
};

// About representation

class GServiceQuota {
public:
    GServiceQuota();
    std::string serviceName;
    long bytesUsed;
    void from_json(JObject* obj);
};

class GFormat {
public:
    GFormat();
    std::string source;
    std::vector<std::string> targets;
    void from_json(JObject* obj);
};

class GRole {
public:
    GRole();
    std::string primaryRole;
    std::vector<std::string> additionalRoles;
    void from_json(JObject* obj);
};

class GRoleInfo {
public:
    GRoleInfo();
    std::string type;
    std::vector<GRole> roleSets;

    void from_json(JObject* obj);
};

class GFeature {
public:
    GFeature();
    std::string featureName;
    double featureRate;

    void from_json(JObject* obj);
};


class GUploadSize {
public:
    GUploadSize();
    std::string type;
    long size;

    void from_json(JObject* obj);
};

class GAbout {
public:
    GAbout();
    void from_json(JObject* obj);

    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::string, name)
    READONLY(GUser, user)
    READONLY(long, quotaBytesTotal)
    READONLY(long, quotaBytesUsed)
    READONLY(long, quotaBytesUsedAggregate)
    READONLY(long, quotaBytesUsedInTrash)
    READONLY(std::string, quotaType)
    READONLY(std::vector<GServiceQuota>, quotaBytesByService)
    READONLY(long, largestChangedId)
    READONLY(long, remainingChangeIds)
    READONLY(std::string, rootFolderId)
    READONLY(std::string, domainSharingPolicy)
    READONLY(std::string, permissionId)
    READONLY(std::vector<GFormat>, importFormats)
    READONLY(std::vector<GFormat>, exportFormats)
    READONLY(std::vector<GRoleInfo>, additionalRoleInfo)
    READONLY(std::vector<GFeature>, features)
    READONLY(std::vector<GUploadSize>, maxUploadSizes)
    READONLY(bool, isCurrentAppInstalled)
    READONLY(std::string, languageCode)
};

// Change representation
class GChange {
public:
    GChange();
    void from_json(JObject* obj);

    READONLY(std::string, id)
    READONLY(std::string, fileId)
    READONLY(std::string, selfLink)
    READONLY(bool, deleted)
    READONLY(struct tm, modificationDate)
    READONLY(GFile, file)
};

class GChangeList {
public:
    GChangeList();
    void from_json(JObject* obj);

    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::string, nextPageToken)
    READONLY(std::string, nextLink)
    READONLY(long, largestChangeId)
    READONLY(std::vector<GChange>, items)
};

// Children representation
class GChildren {
public:
    GChildren();
    void from_json(JObject* obj);
    JObject* to_json();

    std::set<std::string> get_modified_fields() { return _fields;}
    void clear() { _fields.clear();}

    WRITABLE(std::string, id)
    READONLY(std::string, selfLink)
    READONLY(std::string, childLink)
private:
    std::set<std::string> _fields;
};

class GChildrenList {
public:
    GChildrenList();
    void from_json(JObject* obj);

    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::string, nextPageToken)
    READONLY(std::string, nextLink)
    READONLY(std::vector<GChildren>, items)
};


// Revision representation
class GRevision {
public:
    GRevision();
    void from_json(JObject* obj);
    JObject* to_json();

    std::set<std::string> get_modified_fields() { return _fields;}
    void clear() { _fields.clear();}

    READONLY(std::string, etag)
    WRITABLE(std::string, id)
    READONLY(std::string, selfLink)
    READONLY(std::string, mimeType)
    READONLY(struct tm, modifiedDate)
    WRITABLE(bool, pinned)
    READONLY(bool, published)
    READONLY(std::string, publishedLink)
    READONLY(bool, publishedAuto)
    READONLY(bool, publishedOutsideDomain)
    READONLY(std::string, downloadUri)
    READONLY(Links, exportLinks)
    READONLY(std::string, lastModifyingUserName)
    READONLY(GUser, lastModifyingUser)
    READONLY(std::string, originalFilename)
    READONLY(std::string, md5Checksum)
    READONLY(long, fileSize)

private:
    std::set<std::string> _fields;
};

class GRevisionList {
public:
    GRevisionList();
    void from_json(JObject* obj);

    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::vector<GRevision>, items)
};


class GAppIcon {
public:
    GAppIcon();
    void from_json(JObject* obj);
    READONLY(std::string, category)
    READONLY(int, size)
    READONLY(std::string, iconUrl)
};

class GApp {
public:
    GApp();
    void from_json(JObject* obj);
    READONLY(std::string, id)
    READONLY(std::string, name)
    READONLY(std::string, objectType)
    READONLY(std::string, shortDescription)
    READONLY(std::string, longDescription)
    READONLY(bool, supportsCreate)
    READONLY(bool, supportsImport)
    READONLY(bool, supportsMultiOpen)
    READONLY(bool, supportsOfflineCreate)
    READONLY(bool, installed)
    READONLY(bool, authorized)
    READONLY(bool, hasDriveWideScope)
    READONLY(bool, useByDefault)
    READONLY(std::string, productUrl)
    READONLY(std::string, productId)
    READONLY(std::string, openUrlTemplate)
    READONLY(std::string, createUrl)
    READONLY(std::string, createInFolderTemplate)
    READONLY(std::vector<std::string>, primaryMimeTypes)
    READONLY(std::vector<std::string>, secondaryMimeTypes)
    READONLY(std::vector<std::string>, primaryFileExtensions)
    READONLY(std::vector<std::string>, secondaryFileExtensions)
    READONLY(std::vector<GAppIcon>, icons)
};

class GAppList {
public:
    GAppList();
    void from_json(JObject* obj);

    READONLY(std::string, etag)
    READONLY(std::string, selfLink)
    READONLY(std::vector<GApp>, items)
    READONLY(std::vector<std::string>, defaultAppIds)
};

class GReply {
public:
    GReply();
    void from_json(JObject* obj);
    JObject* to_json();

    READONLY(std::string, replyId)
    READONLY(struct tm, createDate)
    READONLY(struct tm, modifiedDate)
    READONLY(GUser, author)
    READONLY(std::string, htmlContent)
    WRITABLE(std::string, content)
    READONLY(bool, deleted)
    WRITABLE(std::string, verb);

    std::set<std::string> get_modified_fields() { return _fields;}
    void clear() { _fields.clear();}
private:
    std::set<std::string> _fields;
};

class GReplyList {
public:
    GReplyList();
    void from_json(JObject* obj);

    READONLY(std::string, selfLink)
    READONLY(std::string, nextPageToken)
    READONLY(std::string, nextLink)
    READONLY(std::vector<GReply>, items)
};


class GCommentContext {
public:
    GCommentContext();
    void from_json(JObject* obj);
    JObject* to_json();

    READONLY(std::string, type)
    READONLY(std::string, value)
};

class GComment {
public:
    GComment();
    void from_json(JObject* obj);
    JObject* to_json();

    READONLY(std::string, selfLink)
    READONLY(std::string, commentId)
    READONLY(struct tm, createdDate)
    READONLY(struct tm, modifiedDate)
    READONLY(GUser, author)
    READONLY(std::string, htmlContent)
    READONLY(std::string, content)
    READONLY(bool, deleted)
    READONLY(std::string, status)
    READONLY(GCommentContext, context)
    READONLY(std::string, anchor)
    READONLY(std::string, fileId)
    READONLY(std::string, fileTitle)
    READONLY(std::vector<GReply>, replies)

    std::set<std::string> get_modified_fields() { return _fields;}
    void clear() { _fields.clear();}
private:
    std::set<std::string> _fields;
};

class GCommentList  {
public:
    GCommentList();
    void from_json(JObject* obj);

    READONLY(std::string, selfLink)
    READONLY(std::string, nextPageToken)
    READONLY(std::string, nextLink)
    READONLY(std::vector<GComment>, items)
};


class GError {
public:
    GError();
    void from_json(JObject* obj);

    READONLY(int, code)
    READONLY(std::string, message)
    READONLY(std::vector<string_map>, errors)
};

}
#endif
