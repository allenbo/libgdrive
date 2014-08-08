#include "gdrive/gitem.hpp"
using namespace JCONER;
namespace GDRIVE {

#define BOOL_FROM_JSON(name) do { \
    if (obj->contain(#name)) {\
        JValue* rst = obj->get(#name); \
        if (rst->type() == VT_TRUE) name = true; \
        else name = false;\
    }\
    }while(0)

#define STRING_FROM_JSON(name) do {\
    if (obj->contain(#name)) {\
        name = ((JString*)obj->get(#name))->getValue(); \
    }\
    }while(0)

#define REAL_FROM_JSON(name) do {\
    if (obj->contain(#name)) {\
        name = ((JReal*)obj->get(#name))->getValue(); \
    }\
    }while(0)

#define INT_FROM_JSON(name) do {\
    if (obj->contain(#name)) {\
        name = ((JInt*)obj->get(#name))->getValue(); \
    }\
    }while(0)

#define INSTANCE_FROM_JSON(name) do {\
    if (obj->contain(#name)) {\
        name.from_json((JObject*)obj->get(#name));\
    }\
    }while(0)

#define INSTANCE_VECTOR_FROM_JSON(type, name) do {\
    if (obj->contain(#name)) {\
        JArray* _1 = (JArray*)obj->get(#name); \
        for (int i = 0; i < _1->size(); i ++) {\
            type _2; \
            _2.from_json((JObject*)_1->get(i)); \
            name.push_back(_2);\
        }\
    }\
    }while(0)

#define STRING_MAP_FROM_JSON(name) do {\
    if (obj->contain(#name)) {\
        JObject* _1 = (JObject*)obj->get(#name);\
        std::vector<std::string> keys = _1->getKeys(); \
        for(int i = 0; i < keys.size(); i ++ ) {\
            std::string key = keys[i]; \
            name[key] = ((JString*)_1->get(key))->getValue();\
        }\
    }\
    }while(0)

#define STRING_VECTOR_FROM_JSON(name) do {\
    if (obj->contain(#name)) {\
        JArray* _1 = (JArray*)obj->get(#name);\
        for(int i = 0; i < _1->size(); i ++ ) {\
            name.push_back(((JString*)_1->get(i))->getValue());\
        }\
    }\
    }while(0)

#define TIME_FROM_JSON(name) do { \
    if (obj->contain(#name)) {\
        std::string repr = ((JString*)obj->get(#name))->getValue();\
        name = time_from_string(repr); \
    }\
    }while(0)

#define BOOL_TO_JSON(name) do {\
    if (name) obj->put(#name, new JTrue());\
    else obj->put(#name, new JFalse());\
    }while(0)

#define STRING_TO_JSON(name) do {\
    if (name != "") obj->put(#name, new JString(name));\
    }while(0)

#define INT_TO_JSON(name) do {\
    if (name != -1) obj->put(#name, new JInt(name));\
    } while(0)

#define REAL_TO_JSON(name) do {\
    if (name != 0.0) obj->put(#name, new JReal(name));\
    } while(0)

#define STRING_VECTOR_TO_JSON(name) do {\
    if (name.size() != 0) { \
        JArray* _1 = new JArray(); \
        for(int i = 0; i < name.size(); i ++ ) { \
            _1->append(name[i]); \
        } \
        obj->put(#name, _1); \
    } \
    }while(0)

#define STRING_MAP_TO_JSON(name) do {\
    if (name.size() != 0) {\
        JObject* _1 = new JObject(); \
        for (std::map<std::string, std::string>::iterator iter = name.begin(); \
                iter != name.end(); iter ++ ) { \
            _1->put(iter->first, iter->second);\
        }\
        obj->put(#name, _1);\
    }\
    }while(0)

#define INSTANCE_TO_JSON(name) do {\
    obj->put(#name, name.to_json()); \
    }while(0)

#define INSTANCE_VECTOR_TO_JSON(name) do {\
    if (name.size() != 0) {\
        JArray* _1 = new JArray(); \
        for(int i = 0; i < name.size(); i ++ ) {\
            _1->append(name[i].to_json()); \
        }\
        obj->put(#name, _1); \
    }\
    }while(0)

#define TIME_TO_JSON(name) do { \
    if (name.tm_year != 0 || name.tm_mon != 0) {\
        obj->put(#name, time_to_string(name)); \
    }\
    }while(0)

struct tm time_from_string(std::string time_repr ) {
    struct tm time;
    sscanf(time_repr.c_str(), "%4d-%2d-%2dT%2d:%2d:%2d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900;
    time.tm_mon -= 1;
    return time;
}

std::string time_to_string(struct tm time) {
    char tmp[512];
    struct tm another = time;
    another.tm_year += 1900;
    another.tm_mon  += 1;
    sprintf(tmp, "%4d-%2d-%2dT%2d:%2d:%2d.000", another.tm_year, another.tm_mon, another.tm_mday, another.tm_hour,another.tm_min, another.tm_sec);
    std::string rst(tmp);
    return rst;
}

GFileLabel::GFileLabel() {
    starred = hidden = trashed = restricted = viewed = false;
}

void GFileLabel::from_json(JObject* obj) {
    BOOL_FROM_JSON(starred);
    BOOL_FROM_JSON(hidden);
    BOOL_FROM_JSON(trashed);
    BOOL_FROM_JSON(restricted);
    BOOL_FROM_JSON(viewed);
}

JObject* GFileLabel::to_json() {
    JObject* obj = new JObject();
    BOOL_TO_JSON(starred);
    BOOL_TO_JSON(hidden);
    BOOL_TO_JSON(trashed);
    BOOL_TO_JSON(restricted);
    BOOL_TO_JSON(viewed);
    return obj;
}

GUser::GUser() {
    displayName = picture_url = permissionId = "";
    isAuthenticatedUser = false;
}

void GUser::from_json(JObject* obj) {
    STRING_FROM_JSON(displayName);
    BOOL_FROM_JSON(isAuthenticatedUser);
    STRING_FROM_JSON(permissionId);
    if (obj->contain("picture")) {
        JObject* item = (JObject*)obj->get("picture");
        picture_url = ((JString*)item->get("url"))->getValue();
    }
}

JObject* GUser::to_json(){
    JObject* obj = new JObject();
    STRING_TO_JSON(displayName);
    BOOL_TO_JSON(isAuthenticatedUser);
    STRING_TO_JSON(permissionId);
    // exception for picture url
    JObject* tmp = new JObject();
    tmp->put("url", picture_url);
    obj->put("picture", tmp);
    return obj;
}

GParent::GParent() {
    id = selfLink = parentLink = "";
    isRoot = false;
}

void GParent::from_json(JObject* obj) {
    STRING_FROM_JSON(id);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(parentLink);
    BOOL_FROM_JSON(isRoot);
}

JObject* GParent::to_json() {
    JObject* obj = new JObject();
    STRING_TO_JSON(id);
    STRING_TO_JSON(selfLink);
    STRING_TO_JSON(parentLink);
    BOOL_TO_JSON(isRoot);
    return obj;
}

GParentList::GParentList() {
    etag = selfLink = "";
    items.clear();
}

void GParentList::from_json(JObject* obj) {
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(selfLink);
    INSTANCE_VECTOR_FROM_JSON(GParent, items);
}

GProperty::GProperty() {
    etag = selfLink = key = visibility = value = "";
}

void GProperty::from_json(JObject* obj) {
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(key);
    STRING_FROM_JSON(visibility);
    STRING_FROM_JSON(value);
}

JObject* GProperty::to_json(){
    JObject* obj = new JObject();
    STRING_TO_JSON(etag);
    STRING_TO_JSON(selfLink);
    STRING_TO_JSON(key);
    STRING_TO_JSON(value);
    STRING_TO_JSON(visibility);
    return obj;
}

GPermission::GPermission() {
    etag = id = selfLink = name = emailAddress = domain = role = "";
    type = value = authKey = photoLink = "";
    withLink = false;
    additionalRoles.clear();
    _fields.clear();
}

void GPermission::from_json(JObject* obj) {
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(id);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(name);
    STRING_FROM_JSON(emailAddress);
    STRING_FROM_JSON(domain);
    STRING_FROM_JSON(role);
    STRING_VECTOR_FROM_JSON(additionalRoles);
    STRING_FROM_JSON(type);
    STRING_FROM_JSON(value);
    STRING_FROM_JSON(authKey);
    BOOL_FROM_JSON(withLink);
    STRING_FROM_JSON(photoLink);
}

GPermissionList::GPermissionList() {
    etag = selfLink = "";
    items.clear();
}

void GPermissionList::from_json(JObject* obj) {
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(selfLink);
    INSTANCE_VECTOR_FROM_JSON(GPermission, items);
}

JObject* GPermission::to_json() {
    JObject* obj = new JObject();
    STRING_TO_JSON(etag);
    STRING_TO_JSON(id);
    STRING_TO_JSON(selfLink);
    STRING_TO_JSON(name);
    STRING_TO_JSON(emailAddress);
    STRING_TO_JSON(domain);
    STRING_TO_JSON(role);
    STRING_VECTOR_TO_JSON(additionalRoles);
    STRING_TO_JSON(type);
    STRING_TO_JSON(value);
    STRING_TO_JSON(authKey);
    BOOL_TO_JSON(withLink);
    STRING_TO_JSON(photoLink);
    return obj;
}
GImageMediaMetaData::GImageMediaMetaData() {
    width = height = rotation = -1; 
    location.latitude = location.longitude = location.altitude = 0.0;
    date = cameraMaker = cameraModel = "";
    exposureTime = aperture = focalLength = 0.0;
    flashUsed = false;
    isoSpeed = -1;
    meteringMode = sensor = exposureMode = colorSpace = whiteBalance = lens = "";
    exposureBias = maxApertureValue = 0.0;
    subjectDistance = -1;
}

void GImageMediaMetaData::Location::from_json(JObject* obj) {
    REAL_FROM_JSON(latitude);
    REAL_FROM_JSON(longitude);
    REAL_FROM_JSON(altitude);
}

JObject* GImageMediaMetaData::Location::to_json() {
    JObject* obj = new JObject();
    REAL_TO_JSON(latitude);
    REAL_TO_JSON(longitude);
    REAL_TO_JSON(altitude);
    return obj;
}

void GImageMediaMetaData::from_json(JObject* obj) {
    INT_FROM_JSON(width);
    INT_FROM_JSON(height);
    INT_FROM_JSON(rotation);
    INSTANCE_FROM_JSON(location);
    STRING_FROM_JSON(date);
    STRING_FROM_JSON(cameraMaker);
    STRING_FROM_JSON(cameraModel);
    REAL_FROM_JSON(exposureTime);
    REAL_FROM_JSON(aperture);
    BOOL_FROM_JSON(flashUsed);
    REAL_FROM_JSON(focalLength);
    BOOL_FROM_JSON(isoSpeed);
    STRING_FROM_JSON(meteringMode);
    STRING_FROM_JSON(sensor);
    STRING_FROM_JSON(exposureMode);
    STRING_FROM_JSON(colorSpace);
    STRING_FROM_JSON(whiteBalance);
    REAL_FROM_JSON(exposureBias);
    REAL_FROM_JSON(maxApertureValue);
    INT_FROM_JSON(subjectDistance);
    STRING_FROM_JSON(lens);
}

JObject* GImageMediaMetaData::to_json() {
    JObject* obj = new JObject();
    INT_TO_JSON(width);
    INT_TO_JSON(height);
    INT_TO_JSON(rotation);
    INSTANCE_TO_JSON(location);
    STRING_TO_JSON(date);
    STRING_TO_JSON(cameraMaker);
    STRING_TO_JSON(cameraModel);
    REAL_TO_JSON(exposureTime);
    REAL_TO_JSON(aperture);
    BOOL_TO_JSON(flashUsed);
    REAL_TO_JSON(focalLength);
    INT_TO_JSON(isoSpeed);
    STRING_TO_JSON(meteringMode);
    STRING_TO_JSON(sensor);
    STRING_TO_JSON(exposureMode);
    STRING_TO_JSON(colorSpace);
    STRING_TO_JSON(whiteBalance);
    REAL_TO_JSON(exposureBias);
    REAL_TO_JSON(maxApertureValue);
    INT_TO_JSON(subjectDistance);
    STRING_TO_JSON(lens);
    return obj;
}

GFile::GFile() {
    id = etag = selfLink = webContentLink = alternateLink = embedLink = "";
    openWithLinks.clear();
    defaultOpenWithLink = iconLink = thumbnailLink = title = mimeType = description = version = "";
    parents.clear();
    exportLinks.clear();
    indexableText = originalFilename = fileExtension = md5Checksum = "";
    permissions.clear();
    fileSize = quotaBytesUsed = -1;
    ownerNames.clear();
    owners.clear();
    lastModifyingUserName = "";
    editable = copyable = writersCanShare = shared = explicitlyTrashed = appDataContents = false;
    headRevisionId = "";
    properties.clear();
}

void GFile::from_json(JObject* obj) {
    STRING_FROM_JSON(id);
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(webContentLink);
    STRING_FROM_JSON(alternateLink);
    STRING_FROM_JSON(embedLink);
    STRING_FROM_JSON(defaultOpenWithLink);
    STRING_MAP_FROM_JSON(openWithLinks);
    STRING_FROM_JSON(iconLink);
    STRING_FROM_JSON(thumbnailLink);
    STRING_FROM_JSON(title);
    STRING_FROM_JSON(mimeType);
    STRING_FROM_JSON(description);
    INSTANCE_FROM_JSON(labels);
    TIME_FROM_JSON(createdDate);
    TIME_FROM_JSON(modifiedDate);
    TIME_FROM_JSON(modifiedByMeDate);
    TIME_FROM_JSON(lastViewedByMeDate);
    TIME_FROM_JSON(sharedWithMeDate);
    STRING_FROM_JSON(version);
    INSTANCE_FROM_JSON(sharingUser);
    INSTANCE_VECTOR_FROM_JSON(GParent, parents);
    STRING_MAP_FROM_JSON(exportLinks);
    STRING_FROM_JSON(indexableText);
    INSTANCE_FROM_JSON(userPermission);
    INSTANCE_VECTOR_FROM_JSON(GPermission, permissions);
    STRING_FROM_JSON(originalFilename);
    STRING_FROM_JSON(fileExtension);
    STRING_FROM_JSON(md5Checksum);
    INT_FROM_JSON(fileSize);
    INT_FROM_JSON(quotaBytesUsed);
    STRING_VECTOR_FROM_JSON(ownerNames);
    INSTANCE_VECTOR_FROM_JSON(GUser, owners);
    STRING_FROM_JSON(lastModifyingUserName);
    INSTANCE_FROM_JSON(lastModifyingUser);
    BOOL_FROM_JSON(editable);
    BOOL_FROM_JSON(copyable);
    BOOL_FROM_JSON(writersCanShare);
    BOOL_FROM_JSON(shared);
    BOOL_FROM_JSON(explicitlyTrashed);
    BOOL_FROM_JSON(appDataContents);
    STRING_FROM_JSON(headRevisionId);
    INSTANCE_VECTOR_FROM_JSON(GProperty, properties);
    INSTANCE_FROM_JSON(imageMediaMetadata);
}

JObject* GFile::to_json() {
    JObject* obj = new JObject();
    STRING_TO_JSON(id);
    STRING_TO_JSON(etag);
    STRING_TO_JSON(selfLink);
    STRING_TO_JSON(webContentLink);
    STRING_TO_JSON(alternateLink);
    STRING_TO_JSON(embedLink);
    STRING_MAP_TO_JSON(openWithLinks);
    STRING_TO_JSON(defaultOpenWithLink);
    STRING_TO_JSON(iconLink);
    STRING_TO_JSON(thumbnailLink);
    STRING_TO_JSON(title);
    STRING_TO_JSON(mimeType);
    STRING_TO_JSON(description);
    INSTANCE_TO_JSON(labels);
    TIME_TO_JSON(createdDate);
    TIME_TO_JSON(modifiedDate);
    TIME_TO_JSON(modifiedByMeDate);
    TIME_TO_JSON(lastViewedByMeDate);
    TIME_TO_JSON(sharedWithMeDate);
    STRING_TO_JSON(version);
    INSTANCE_TO_JSON(sharingUser);
    INSTANCE_VECTOR_TO_JSON(parents);
    STRING_MAP_TO_JSON(exportLinks);
    STRING_TO_JSON(indexableText);
    INSTANCE_TO_JSON(userPermission);
    INSTANCE_VECTOR_TO_JSON(permissions);
    STRING_TO_JSON(originalFilename);
    STRING_TO_JSON(fileExtension);
    STRING_TO_JSON(md5Checksum);
    INT_TO_JSON(fileSize);
    INT_TO_JSON(quotaBytesUsed);
    STRING_VECTOR_TO_JSON(ownerNames);
    INSTANCE_VECTOR_TO_JSON(owners);
    STRING_TO_JSON(lastModifyingUserName);
    INSTANCE_TO_JSON(lastModifyingUser);
    BOOL_TO_JSON(editable);
    BOOL_TO_JSON(copyable);
    BOOL_TO_JSON(writersCanShare);
    BOOL_TO_JSON(shared);
    BOOL_TO_JSON(explicitlyTrashed);
    BOOL_TO_JSON(appDataContents);
    STRING_TO_JSON(headRevisionId);
    INSTANCE_VECTOR_TO_JSON(properties);
    INSTANCE_TO_JSON(imageMediaMetadata);
    return obj;
}


GFileList::GFileList() {
    etag = selfLink = nextPageToken = nextLink = "";
    items.clear();
}

void GFileList::from_json(JObject* obj) {
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(nextPageToken);
    STRING_FROM_JSON(nextLink);
    INSTANCE_VECTOR_FROM_JSON(GFile, items);
}


GServiceQuota::GServiceQuota(){
    serviceName = "";
    bytesUsed = -1;
}

void GServiceQuota::from_json(JObject* obj) {
    STRING_FROM_JSON(serviceName);
    INT_FROM_JSON(bytesUsed);
}

GFormat::GFormat() {
    source = "";
    targets.clear();
}

void GFormat::from_json(JObject* obj) {
    STRING_FROM_JSON(source);
    STRING_VECTOR_FROM_JSON(targets);
}

GRole::GRole() {
    primaryRole = "";
    additionalRoles.clear();
}

void GRole::from_json(JObject* obj) {
    STRING_FROM_JSON(primaryRole);
    STRING_VECTOR_FROM_JSON(additionalRoles);
}

GRoleInfo::GRoleInfo() {
    type = "";
    roleSets.clear();
}

void GRoleInfo::from_json(JObject* obj) {
    STRING_FROM_JSON(type);
    INSTANCE_VECTOR_FROM_JSON(GRole, roleSets);
}

GFeature::GFeature() {
    featureName = "";
    featureRate = 0.0;
}

void GFeature::from_json(JObject* obj) {
    STRING_FROM_JSON(featureName);
    REAL_FROM_JSON(featureRate);
}

GUploadSize::GUploadSize () {
    type = "";
    size = -1;
}

void GUploadSize::from_json(JObject* obj) {
    STRING_FROM_JSON(type);
    INT_FROM_JSON(size);
}

GAbout::GAbout() {
    etag = selfLink = name = "";
    quotaBytesTotal = quotaBytesUsed = quotaBytesUsedAggregate = quotaBytesUsedInTrash = -1;
    quotaType = "";
    quotaBytesByService.clear();
    largestChangedId = remainingChangeIds = -1;
    rootFolderId = domainSharingPolicy = permissionId = "";
    importFormats.clear();
    exportFormats.clear();
    additionalRoleInfo.clear();
    features.clear();
    maxUploadSizes.clear();
    isCurrentAppInstalled = false;
    languageCode = "";
}

void GAbout::from_json(JObject* obj) {
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(name);
    INSTANCE_FROM_JSON(user);
    INT_FROM_JSON(quotaBytesTotal);
    INT_FROM_JSON(quotaBytesUsed);
    INT_FROM_JSON(quotaBytesUsedAggregate);
    INT_FROM_JSON(quotaBytesUsedInTrash);
    STRING_FROM_JSON(quotaType);
    INSTANCE_VECTOR_FROM_JSON(GServiceQuota,quotaBytesByService);
    INT_FROM_JSON(largestChangedId);
    INT_FROM_JSON(remainingChangeIds);
    STRING_FROM_JSON(rootFolderId);
    STRING_FROM_JSON(domainSharingPolicy);
    STRING_FROM_JSON(permissionId);
    INSTANCE_VECTOR_FROM_JSON(GFormat, importFormats);
    INSTANCE_VECTOR_FROM_JSON(GFormat, exportFormats);
    INSTANCE_VECTOR_FROM_JSON(GRoleInfo, additionalRoleInfo);
    INSTANCE_VECTOR_FROM_JSON(GFeature, features);
    INSTANCE_VECTOR_FROM_JSON(GUploadSize, maxUploadSizes);
    BOOL_FROM_JSON(isCurrentAppInstalled);
    STRING_FROM_JSON(languageCode);
}

GChange::GChange() {
    id = fileId = selfLink = "";
    deleted = false;
}

void GChange::from_json(JObject* obj) {
    STRING_FROM_JSON(id);
    STRING_FROM_JSON(fileId);
    STRING_FROM_JSON(selfLink);
    BOOL_FROM_JSON(deleted);
    TIME_FROM_JSON(modificationDate);
    INSTANCE_FROM_JSON(file);
}

GChangeList::GChangeList() {
    etag = selfLink = nextPageToken = nextLink = "";
    largestChangeId = -1;
    items.clear();
}

void GChangeList::from_json(JObject* obj) {
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(nextPageToken);
    STRING_FROM_JSON(nextLink);
    INT_FROM_JSON(largestChangeId);
    INSTANCE_VECTOR_FROM_JSON(GChange, items);
}

GChildren::GChildren() {
    id = selfLink = childLink = "";
}

void GChildren::from_json(JObject* obj) {
    STRING_FROM_JSON(id);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(childLink);
}

JObject* GChildren::to_json() {
    JObject* obj = new JObject();
    STRING_TO_JSON(id);
    STRING_TO_JSON(selfLink);
    STRING_TO_JSON(childLink);
    return obj;
}

GChildrenList::GChildrenList() {
    etag = selfLink = nextPageToken = nextLink = "";
    items.clear();
}

void GChildrenList::from_json(JObject* obj) {
    STRING_FROM_JSON(etag);
    STRING_FROM_JSON(selfLink);
    STRING_FROM_JSON(nextPageToken);
    STRING_FROM_JSON(nextLink);
    INSTANCE_VECTOR_FROM_JSON(GChildren, items);
}

}
