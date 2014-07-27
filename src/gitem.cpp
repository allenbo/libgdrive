#include "gdrive/gitem.hpp"
using namespace JCONER;
namespace GDRIVE {

struct tm time_convert(std::string time_repr ) {
    struct tm time;
    sscanf(time_repr.c_str(), "%4d-%2d-%2dT%2d:%2d:%2d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900;
    time.tm_mon -= 1;
    return time;
}

GFileLabel::GFileLabel() {
    starred = hidden = trashed = restricted = viewed = false;
}

void GFileLabel::from_json(JObject* obj) {
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
}

GUser::GUser() {
    display_name = picture_url = permission_id = "";
    isAuthenticated = false;
}

void GUser::from_json(JObject* obj) {
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
    if (obj->contain("picture")) {
        JObject* item = (JObject*)obj->get("picture");
        picture_url = ((JString*)item->get("url"))->getValue();
    }
}

GParent::GParent() {
    id = self_link = parent_link = "";
    is_root = false;
}

void GParent::from_json(JObject* obj) {
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
}

GProperty::GProperty() {
    etag = self_link = key = visibility = value = "";
}

void GProperty::from_json(JObject* obj) {
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
}

GPermission::GPermission() {
    etag = id = self_link = name = email_address = domain = role = "";
    type = value = auth_key = with_link = photo_link = "";
    additional_roles.clear();
}

void GPermission::from_json(JObject* obj) {
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
}

GImageMediaMetaData::GImageMediaMetaData() {
    width = height = rotation = -1; 
    location.latitude = location.longitude = location.altitude = 0.0;
    date = camera_maker = camera_model = "";
    exposure_time = aperture = focal_length = 0.0;
    flash_used = false;
    iso_speed = -1;
    metering_mode = sensor = exposure_mode = color_space = white_balance = lens = "";
    exposure_bias = max_aperture_value = 0.0;
    subject_distance = -1;
}

void GImageMediaMetaData::Location::from_json(JObject* obj) {
    if (obj->contain("latitude")) {
        latitude = ((JReal*)obj->get("latitude"))->getValue();
    }
    if (obj->contain("longitude")) {
        longitude = ((JReal*)obj->get("longitude"))->getValue();
    }
    if (obj->contain("altitude")) {
        altitude = ((JReal*)obj->get("altitude"))->getValue();
    }
}

void GImageMediaMetaData::from_json(JObject* obj) {
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
}

GFile::GFile() {
    id = etag = self_link = web_content_link = alternate_link = embed_link = "";
    open_with_links.clear();
    default_open_with_link = icon_link = thumbnail_link = title = mime_type = description = version = "";
    parents.clear();
    export_links.clear();
    indexable_text = original_filename = file_extension = md5_checksum = "";
    permissions.clear();
    file_size = quota_bytes_used = 0;
    owner_names.clear();
    owners.clear();
    last_modifying_username = "";
    editable = copyable = writers_can_share = shared = explicitly_trashed = app_data_contents = false;
    head_revision_id = "";
    properties.clear();
}

void GFile::from_json(JObject* obj) {
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
}

}
