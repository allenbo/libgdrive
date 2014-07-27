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
};

struct GUser {
    GUser();
    std::string display_name;
    std::string picture_url;
    bool isAuthenticated;
    std::string permission_id;
    void from_json(JObject* obj);
};

struct GParent {
    GParent();
    std::string id;
    std::string self_link;
    std::string parent_link;
    bool is_root;
    void from_json(JObject* obj);
};


struct GProperty {
    GProperty();
    std::string etag;
    std::string self_link;
    std::string key;
    std::string visibility;
    std::string value;
    void from_json(JObject* obj);
};

struct GPermission {
    GPermission();
    std::string etag;
    std::string id;
    std::string self_link;
    std::string name;
    std::string email_address;
    std::string domain;
    std::string role;
    std::vector<std::string> additional_roles;
    std::string type;
    std::string value;
    std::string auth_key;
    std::string with_link;
    std::string photo_link;
    void from_json(JObject* obj);
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
    } location;
    std::string date;
    std::string camera_maker;
    std::string camera_model;
    double exposure_time;
    double aperture;
    bool flash_used;
    double focal_length;
    int iso_speed;
    std::string metering_mode;
    std::string sensor;
    std::string exposure_mode;
    std::string color_space;
    std::string white_balance;
    double exposure_bias;
    double max_aperture_value;
    int subject_distance;
    std::string lens;
    void from_json(JObject* obj);
};

typedef std::map<std::string, std::string> GExportLink;

struct GFile {
    GFile();
    std::string id;
    std::string etag;
    std::string self_link;
    std::string web_content_link;
    std::string alternate_link;
    std::string embed_link;
    std::map<std::string, std::string> open_with_links;
    std::string default_open_with_link;
    std::string icon_link;
    std::string thumbnail_link;
    //thumbnail
    std::string title;
    std::string mime_type;
    std::string description;
    GFileLabel labels;
    struct tm created_date;
    struct tm modified_date;
    struct tm modifiedbyme_date;
    struct tm lastviewedbyme_date;
    struct tm sharedwithme_date;
    std::string version;
    GUser sharing_user;
    std::vector<GParent> parents;
    GExportLink export_links;
    std::string indexable_text;
    GPermission user_permission;
    std::vector<GPermission> permissions;
    std::string original_filename;
    std::string file_extension;
    std::string md5_checksum;
    long file_size;
    int quota_bytes_used;
    std::vector<std::string> owner_names;
    std::vector<GUser> owners;
    std::string last_modifying_username;
    GUser last_modifying_user;
    bool editable;
    bool copyable;
    bool writers_can_share;
    bool shared;
    bool explicitly_trashed;
    bool app_data_contents;
    std::string head_revision_id;
    std::vector<GProperty> properties;
    GImageMediaMetaData image_media_meta_data;

    void from_json(JObject* obj);
};

}

#endif
