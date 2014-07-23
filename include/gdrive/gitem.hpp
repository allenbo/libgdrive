#ifndef __GDRIVE_GFILE_HPP__
#define __GDRIVE_GFILE_HPP__

#include <string>
#include <time.h>
#include "jconer/json.hpp"

using namespace JCONER;

namespace GDRIVE {

struct GFileLabel {
    bool starred;
    bool hidden;
    bool trashed;
    bool restricted;
    bool viewed;
    void from_json(JObject* obj);
};

struct GUser {
    std::string display_name;
    std::string picture_url;
    bool isAuthenticated;
    std::string permission_id;
    void from_json(JObject* obj);
};

struct GParent {
};

struct GUserPermission {
    std::string etag;
    std::string id;
    std::string self_link;
    std::string role;
    std::string type;
    void from_json(JObject* obj);
};

typedef std::map<std::string, std::string> GExportLink;

struct GFile {
    std::string id;
    std::string etag;
    std::string self_link;
    std::string alternate_link;
    std::string embed_link;
    std::string icon_link;
    std::string thumbnail_link;
    std::string title;
    std::string mime_type;
    GFileLabel labels;
    time_t created_date;
    time_t modified_date;
    time_t sharedwithme_date;
    std::string version;
    GUser sharing_user;
    std::vector<GParent> parents;
    GExportLink export_links;
    GUserPermission user_permission;
    int quota_bytes_used;
    std::vector<std::string> owner_names;
    std::vector<GUser> owners;
    std::string last_modifying_username;
    GUser last_modifying_user;
    bool editable;
    bool copyable;
    bool writers_can_share;
    bool shared;
    bool app_data_contents;


    void from_json(JObject* obj);
};

}

#endif
