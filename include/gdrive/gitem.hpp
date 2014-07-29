#ifndef __GDRIVE_GFILE_HPP__
#define __GDRIVE_GFILE_HPP__

#include <string>
#include <time.h>
#include "jconer/json.hpp"

using namespace JCONER;

namespace GDRIVE {

struct GFileLabel {
    GFileLabel();
    bool starred;
    bool hidden;
    bool trashed;
    bool restricted;
    bool viewed;
    void from_json(JObject* obj);
    JObject* to_json();
};

struct GUser {
    GUser();
    std::string displayName;
    std::string picture_url;
    bool isAuthenticatedUser;
    std::string permissionId;
    void from_json(JObject* obj);
    JObject* to_json();
};

struct GParent {
    GParent();
    std::string id;
    std::string selfLink;
    std::string parentLink;
    bool isRoot;
    void from_json(JObject* obj);
    JObject* to_json();
};


struct GProperty {
    GProperty();
    std::string etag;
    std::string selfLink;
    std::string key;
    std::string visibility;
    std::string value;
    void from_json(JObject* obj);
    JObject* to_json();
};

struct GPermission {
    GPermission();
    std::string etag;
    std::string id;
    std::string selfLink;
    std::string name;
    std::string emailAddress;
    std::string domain;
    std::string role;
    std::vector<std::string> additionalRoles;
    std::string type;
    std::string value;
    std::string authKey;
    std::string withLink;
    std::string photoLink;
    void from_json(JObject* obj);
    JObject* to_json();
};

struct GImageMediaMetaData {
    GImageMediaMetaData();
    int width;
    int height;
    int rotation;
    struct Location {
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

struct GFile {
    GFile();
    std::string id;
    std::string etag;
    std::string selfLink;
    std::string webContentLink;
    std::string alternateLink;
    std::string embedLink;
    std::map<std::string, std::string> openWithLinks;
    std::string defaultOpenWithLink;
    std::string iconLink;
    std::string thumbnailLink;
    //thumbnail
    std::string title;
    std::string mimeType;
    std::string description;
    GFileLabel labels;
    struct tm createdDate;
    struct tm modifiedDate;
    struct tm modifiedByMeDate;
    struct tm lastViewedByMeDate;
    struct tm sharedWithMeDate;
    std::string version;
    GUser sharingUser;
    std::vector<GParent> parents;
    GExportLink exportLinks;
    std::string indexableText;
    GPermission userPermission;
    std::vector<GPermission> permissions;
    std::string originalFilename;
    std::string fileExtension;
    std::string md5Checksum;
    long fileSize;
    int quotaBytesUsed;
    std::vector<std::string> ownerNames;
    std::vector<GUser> owners;
    std::string lastModifyingUserName;
    GUser lastModifyingUser;
    bool editable;
    bool copyable;
    bool writersCanShare;
    bool shared;
    bool explicitlyTrashed;
    bool appDataContents;
    std::string headRevisionId;
    std::vector<GProperty> properties;
    GImageMediaMetaData imageMediaMetadata;

    void from_json(JObject* obj);
    JObject* to_json();
};

}

#endif
