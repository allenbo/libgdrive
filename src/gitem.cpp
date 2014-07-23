#include "gdrive/gitem.hpp"
using namespace JCONER;
namespace GDRIVE {

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

void GUserPermission::from_json(JObject* obj) {
    if (obj->contain("etag")) {
        etag = ((JString*)obj->get("etag"))->getValue();
    }
    if (obj->contain("id")) {
        id = ((JString*)obj->get("id"))->getValue();
    }
    if (obj->contain("selfLink")) {
        self_link = ((JString*)obj->get("selfLink"))->getValue();
    }
    if (obj->contain("role")) {
        role = ((JString*)obj->get("selfLink"))->getValue();
    }
    if (obj->contain("type")) {
        type = ((JString*)obj->get("selfLink"))->getValue();
    }
}

void GFile::from_json(JObject* obj) {
    if (obj->contain("id"))
        id = ((JString*)obj->get("id"))->getValue();
    if (obj->contain("etag"))
        etag = ((JString*)obj->get("etag"))->getValue();
    if (obj->contain("selfLink"))
        self_link = ((JString*)obj->get("selfLink"))->getValue();
    if (obj->contain("alternateLink"))
        alternate_link = ((JString*)obj->get("alternateLink"))->getValue();
    if (obj->contain("embedLink"))
        embed_link = ((JString*)obj->get("embedLink"))->getValue();
    if (obj->contain("iconLink"))
        icon_link = ((JString*)obj->get("iconLink"))->getValue();
    if (obj->contain("thumbnailLink"))
        thumbnail_link = ((JString*)obj->get("thumbnailLink"))->getValue();
    if (obj->contain("title"))
        title = ((JString*)obj->get("title"))->getValue();
    if (obj->contain("mimeType"))
        mime_type = ((JString*)obj->get("mimeType"))->getValue();
    if (obj->contain("labels"))
        labels.from_json((JObject*)obj->get("labels"));
    //TODO time json

    if (obj->contain("version"))
        version = ((JString*)obj->get("version"))->getValue();
    if (obj->contain("sharingUser"))
        sharing_user.from_json((JObject*)obj->get("sharingUser"));
    //TODO parent

    //Link
    if (obj->contain("exportLinks")) {
        JObject *links = (JObject*)obj->get("exportLinks");
        std::vector<std::string> keys = links->getKeys();
        for(int i = 0; i < keys.size(); i ++ ){
            std::string key = keys[i];
            export_links[key] = ((JString*)links->get(key))->getValue();
        }
    }

    if (obj->contain("userPermission"))
        user_permission.from_json((JObject*)obj->get("userPermission"));
    if (obj->contain("quotaBytesUsed"))
        quota_bytes_used = atoi( ((JString*)obj->get("quotaBytesUsed"))->getValue().c_str() );
    
    // owers' names
    if (obj->contain("ownerNames")) {
        JArray* names = (JArray*)obj->get("ownerNames");
        for(int i = 0; i < names->size(); i ++ ) {
            owner_names.push_back( ((JString*)names->get(i))->getValue() );
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

    if (obj->contain("appDataContents")) {
        rst = obj->get("appDataContents");
        if (rst->type() == VT_TRUE) app_data_contents = true;
        else app_data_contents = false;
    }
}

}
