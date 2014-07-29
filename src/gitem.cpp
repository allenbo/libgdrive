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
    /*
    if (obj->contain("starred")) {
        JValue* rst = obj->get("starred");
        if (rst->type() == VT_TRUE) starred = true;
        else starred = false;
    }
    if (obj->contain("hidden")) {
        JValue* rst = obj->get("hidden");
        if (rst->type() == VT_TRUE) hidden = true;
        else hidden = false;
    }
    if (obj->contain("trashed")) {
        JValue* rst = obj->get("trashed");
        if (rst->type() == VT_TRUE) trashed = true;
        else trashed = false;
    }
    if (obj->contain("restricted")) {
        JValue* rst = obj->get("restricted");
        if (rst->type() == VT_TRUE) restricted = true;
        else restricted = false;
    }
    if (obj->contain("viewed")) {
        JValue* rst = obj->get("viewed");
        if (rst->type() == VT_TRUE) viewed = true;
        else viewed = false;
    }
    */
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
    /*
    if (obj->contain("displayName")) {
        display_name = ((JString*)obj->get("displayName"))->getValue();
    }
    if (obj->contain("permissionId")) {
        permission_id = ((JString*)obj->get("permissionId"))->getValue();
    }
    if (obj->contain("isAuthenticatedUser")) {
        JValue* rst = obj->get("isAuthenticatedUser");
        if (rst->type() == VT_TRUE) isAuthenticated = true;
        else isAuthenticated = false;
    }
    */
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
    /*
    if (obj->contain("id")) {
        id = ((JString*)obj->get("id"))->getValue();
    }
    if (obj->contain("selfLink")) {
        self_link = ((JString*)obj->get("selfLink"))->getValue();
    }
    if (obj->contain("parentLink")) {
        parent_link = ((JString*)obj->get("parentLink"))->getValue();
    }
    if (obj->contain("isRoot")) {
        JValue* rst = obj->get("isRoot");
        if (rst->type() == VT_TRUE) is_root = true;
        else is_root = false;
    }
    */
}

JObject* GParent::to_json() {
    JObject* obj = new JObject();
    STRING_TO_JSON(id);
    STRING_TO_JSON(selfLink);
    STRING_TO_JSON(parentLink);
    BOOL_TO_JSON(isRoot);
    return obj;
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
    /*
    if (obj->contain("etag")) {
        etag = ((JString*)obj->get("etag"))->getValue();
    }
    if (obj->contain("selfLink")) {
        self_link = ((JString*)obj->get("selfLink"))->getValue();
    }
    if (obj->contain("key")) {
        key = ((JString*)obj->get("key"))->getValue();
    }
    if (obj->contain("visibility")) {
        visibility = ((JString*)obj->get("visibility"))->getValue();
    }
    if (obj->contain("value")) {
        value = ((JString*)obj->get("value"))->getValue();
    }
    */
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
    type = value = authKey = withLink = photoLink = "";
    additionalRoles.clear();
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
    STRING_FROM_JSON(withLink);
    STRING_FROM_JSON(photoLink);
    /*
    if (obj->contain("etag")) {
        etag = ((JString*)obj->get("etag"))->getValue();
    }
    if (obj->contain("id")) {
        id = ((JString*)obj->get("id"))->getValue();
    }
    if (obj->contain("selfLink")) {
        self_link = ((JString*)obj->get("selfLink"))->getValue();
    }
    if (obj->contain("name")) {
        name = ((JString*)obj->get("name"))->getValue();
    }
    if (obj->contain("emailAddress")) {
        email_address = ((JString*)obj->get("emailAddress"))->getValue();
    }
    if (obj->contain("domain")) {
        domain = ((JString*)obj->get("domain"))->getValue();
    }
    if (obj->contain("role")) {
        role = ((JString*)obj->get("role"))->getValue();
    }
    if (obj->contain("additionalRoles")) {
        JArray* role_array = (JArray*)obj->get("additionalRoles");
        for (int i = 0; i < role_array->size(); i ++ ) {
            additional_roles.push_back( ((JString*)role_array->get(i))->getValue() );
        }
    }
    if (obj->contain("type")) {
        type = ((JString*)obj->get("type"))->getValue();
    }
    if (obj->contain("value")) {
        value = ((JString*)obj->get("value"))->getValue();
    }
    if (obj->contain("authKey")) {
        auth_key = ((JString*)obj->get("authKey"))->getValue();
    }
    if (obj->contain("withLink")) {
        with_link = ((JString*)obj->get("withLink"))->getValue();
    }
    if (obj->contain("photoLink")) {
        photo_link = ((JString*)obj->get("photoLink"))->getValue();
    }
    */
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
    STRING_TO_JSON(withLink);
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
    /*
    if (obj->contain("latitude")) {
        latitude = ((JReal*)obj->get("latitude"))->getValue();
    }
    if (obj->contain("longitude")) {
        longitude = ((JReal*)obj->get("longitude"))->getValue();
    }
    if (obj->contain("altitude")) {
        altitude = ((JReal*)obj->get("altitude"))->getValue();
    }
    */
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
    /*
    if (obj->contain("width")) {
        width = ((JInt*)obj->get("width"))->getValue();
    }
    if (obj->contain("height")) {
        height = ((JInt*)obj->get("height"))->getValue();
    }
    if (obj->contain("rotation")) {
        rotation = ((JInt*)obj->get("rotation"))->getValue();
    }
    if (obj->contain("location")) {
        location.from_json((JObject*)obj->get("location"));
    }

    if (obj->contain("date")) {
        date = ((JString*)obj->get("date"))->getValue();
    }
    if (obj->contain("camera_maker")) {
        camera_maker = ((JString*)obj->get("camera_maker"))->getValue();
    }
    if (obj->contain("camera_model")) {
        camera_model = ((JString*)obj->get("camera_model"))->getValue();
    }
    if (obj->contain("exposureTime")) {
        exposure_time = ((JReal*)obj->get("exposureTime"))->getValue();
    }
    if (obj->contain("aperture")) {
        aperture = ((JReal*)obj->get("aperture"))->getValue();
    }
    if (obj->contain("flashUsed")) {
        JValue* rst = obj->get("flashUsed");
        if (rst->type() == VT_TRUE) flash_used = true;
        else flash_used = false;
    }
    if (obj->contain("focalLength")) {
        focal_length = ((JReal*)obj->get("focalLength"))->getValue();
    }
    if (obj->contain("isoSpeed")) {
        iso_speed = ((JInt*)obj->get("isoSpeed"))->getValue();
    }
    if (obj->contain("meterinMode")) {
        metering_mode = ((JString*)obj->get("meteringMode"))->getValue();
    }
    if (obj->contain("sensor")) {
        sensor = ((JString*)obj->get("sensor"))->getValue();
    }
    if (obj->contain("exposureMode")) {
        exposure_mode = ((JString*)obj->get("exposureMode"))->getValue();
    }
    if (obj->contain("colorSpace")) {
        color_space = ((JString*)obj->get("colorSpace"))->getValue();
    }
    if (obj->contain("whiteBalance")) {
        white_balance = ((JString*)obj->get("whiteBalance"))->getValue();
    }
    if (obj->contain("exposureBias")) {
        exposure_bias = ((JReal*)obj->get("exposureBias"))->getValue();
    }
    if (obj->contain("maxApertureValue")) {
        max_aperture_value = ((JReal*)obj->get("maxApertureValue"))->getValue();
    }
    if (obj->contain("subjectDistance")) {
        subject_distance = ((JInt*)obj->get("subjectDistance"))->getValue();
    }
    if (obj->contain("lens")) {
        lens = ((JString*)obj->get("lens"))->getValue();
    }
    */
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
    /*
    if (obj->contain("id"))
        id = ((JString*)obj->get("id"))->getValue();
    if (obj->contain("etag"))
        etag = ((JString*)obj->get("etag"))->getValue();
    if (obj->contain("selfLink"))
        self_link = ((JString*)obj->get("selfLink"))->getValue();
    if (obj->contain("webContentLink"))
        web_content_link = ((JString*)obj->get("webContentLink"))->getValue();
    if (obj->contain("alternateLink"))
        alternate_link = ((JString*)obj->get("alternateLink"))->getValue();
    if (obj->contain("embedLink"))
        embed_link = ((JString*)obj->get("embedLink"))->getValue();

    if (obj->contain("openWithLinks")) {
        JObject* link_object = (JObject*)obj->get("openWithLinks");
        std::vector<std::string> keys = link_object->getKeys();
        for (int i = 0; i < keys.size(); i ++ ) {
            std::string key = keys[i];
            std::string value = ((JString*)link_object->get(key))->getValue();
            open_with_links[key] = value;
        }
    }

    if (obj->contain("defaultOpenWithLink"))
        default_open_with_link = ((JString*)obj->get("defaultOpenWithLink"))->getValue();
    if (obj->contain("iconLink"))
        icon_link = ((JString*)obj->get("iconLink"))->getValue();
    if (obj->contain("thumbnailLink"))
        thumbnail_link = ((JString*)obj->get("thumbnailLink"))->getValue();
    if (obj->contain("title"))
        title = ((JString*)obj->get("title"))->getValue();
    if (obj->contain("mimeType"))
        mime_type = ((JString*)obj->get("mimeType"))->getValue();
    if (obj->contain("description"))
        description = ((JString*)obj->get("description"))->getValue();
    if (obj->contain("labels"))
        labels.from_json((JObject*)obj->get("labels"));
    //TODO time json
    {
        std::string time_repr;
        if (obj->contain("createdDate")) {
            time_repr = ((JString*)obj->get("createdDate"))->getValue();
            created_date = time_convert(time_repr);
        }
        if (obj->contain("modifiedDate")) {
            time_repr = ((JString*)obj->get("modifiedDate"))->getValue();
            modified_date = time_convert(time_repr);
        }
        if (obj->contain("modifiedByMeDate")) {
            time_repr = ((JString*)obj->get("modifiedByMeDate"))->getValue();
            modifiedbyme_date = time_convert(time_repr);
        }
        if (obj->contain("lastViewedByMeDate")) {
            time_repr = ((JString*)obj->get("lastViewedByMeDate"))->getValue();
            lastviewedbyme_date = time_convert(time_repr);
        }
        if (obj->contain("sharedWithMeDate")) {
            time_repr = ((JString*)obj->get("sharedWithMeDate"))->getValue();
            sharedwithme_date = time_convert(time_repr);
        }

    }

    if (obj->contain("version"))
        version = ((JString*)obj->get("version"))->getValue();
    if (obj->contain("sharingUser"))
        sharing_user.from_json((JObject*)obj->get("sharingUser"));
    //TODO parent
    if (obj->contain("parents")) {
        JArray* parent_array = (JArray*)obj->get("parents");
        for (int i = 0; i < parent_array->size(); i ++ ) {
            GParent p;
            p.from_json((JObject*)parent_array->get(i));
            parents.push_back(p);
        }
    }
    
    //Link
    if (obj->contain("exportLinks")) {
        JObject *links = (JObject*)obj->get("exportLinks");
        std::vector<std::string> keys = links->getKeys();
        for(int i = 0; i < keys.size(); i ++ ){
            std::string key = keys[i];
            export_links[key] = ((JString*)links->get(key))->getValue();
        }
    }

    if (obj->contain("indexableText"))
        indexable_text = ((JString*)obj->get("indexable_text"))->getValue();
    if (obj->contain("userPermission"))
        user_permission.from_json((JObject*)obj->get("userPermission"));
    if (obj->contain("permissions")) {
        JArray* permission_array = (JArray*)obj->get("permissions");
        for (int i = 0; i < permission_array->size(); i ++ ) {
            GPermission p;
            p.from_json((JObject*)permission_array->get(i));
        }
    }
    if (obj->contain("originalFilename"))
        original_filename = ((JString*)obj->get("originalFilename"))->getValue();
    if (obj->contain("fileExtension"))
        file_extension = ((JString*)obj->get("fileExtension"))->getValue();
    if (obj->contain("md5Checksum"))
        md5_checksum = ((JString*)obj->get("md5Checksum"))->getValue();
    if (obj->contain("fileSize"))
        file_size = ((JInt*)obj->get("fileSize"))->getValue();
    if (obj->contain("quotaBytesUsed"))
        quota_bytes_used = atoi( ((JString*)obj->get("quotaBytesUsed"))->getValue().c_str() );
    
    // owers' names
    if (obj->contain("ownerNames")) {
        JArray* names = (JArray*)obj->get("ownerNames");
        for(int i = 0; i < names->size(); i ++ ) {
            std::string name = ((JString*)names->get(i))->getValue();
            owner_names.push_back(name);
        }
    }

    // owers
    if (obj->contain("owners")) {
    JArray* owners_item = (JArray*)obj->get("owners");
        for(int i = 0; i < owners_item->size(); i ++ ) {
            GUser user;
            user.from_json( (JObject*)owners_item->get(i) );
            owners.push_back(user);
        }
    }

    if (obj->contain("lastModifyingUserName"))
        last_modifying_username = ((JString*)obj->get("lastModifyingUserName"))->getValue();
    if (obj->contain("lastModifyingUser"))
        last_modifying_user.from_json((JObject*)obj->get("lastModifyingUser"));

    JValue* rst = NULL; 
    if (obj->contain("editable")) {
        rst = obj->get("editable");
        if (rst->type() == VT_TRUE) editable = true;
        else editable = false;
    }

    if (obj->contain("copyable")) {
        rst = obj->get("copyable");
        if (rst->type() == VT_TRUE) copyable = true;
        else copyable = false;
    }
    
    if (obj->contain("writersCanShare")) {
        rst = obj->get("writersCanShare");
        if (rst->type() == VT_TRUE) writers_can_share = true;
        else writers_can_share = false;
    }

    if (obj->contain("shared")) {
        rst = obj->get("shared");
        if (rst->type() == VT_TRUE) shared = true;
        else shared = false;
    }

    if (obj->contain("explicitlyTrashed")) {
        rst = obj->get("explicitlyTrashed");
        if (rst->type() == VT_TRUE) explicitly_trashed = true;
        else explicitly_trashed = false;
    }

    if (obj->contain("appDataContents")) {
        rst = obj->get("appDataContents");
        if (rst->type() == VT_TRUE) app_data_contents = true;
        else app_data_contents = false;
    }

    if (obj->contain("headRevisionId")) {
        head_revision_id = ((JString*)obj->get("headRevisionId"))->getValue();
    }

    if (obj->contain("properties")) {
        JArray* property_array = (JArray*)obj->get("properties");
        for(int i = 0; i < property_array->size(); i ++ ) {
            GProperty pro;
            pro.from_json((JObject*)property_array->get(i));
            properties.push_back(pro);
        }
    }

    if (obj->contain("imageMediaMetadata")) {
        image_media_meta_data.from_json((JObject*)obj->get("imageMediaMetadata"));
    }
    */
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

}
