#include "gdrive/gdrive.hpp"
#include <iostream>
#include <assert.h>
#include <vector>

using namespace GDRIVE;

int main() {
    char* user_home = getenv("HOME");
    if (user_home == NULL) {
        fprintf(stderr, "No $HOME environment variable\n");
        exit(-1);
    }

    char default_gdrive_dir[512];
    strcpy(default_gdrive_dir, user_home);
    strcat(default_gdrive_dir, "/.gdrive/data");

    char* gdrive_dir = getenv("GDRIVE");
    if (gdrive_dir == NULL) {
        gdrive_dir = default_gdrive_dir;
    }
    
    FileStore fs(gdrive_dir);
    assert(fs.status() == SS_FULL);

    Credential cred(&fs);

    if (fs.get("refresh_token") == "") {
        std::string client_id = fs.get("client_id");
        std::string client_secret = fs.get("client_secret");

        OAuth oauth(client_id, client_secret);    
        std::cout << "Please go to this url using your browser, after you authorize this application, you will get a code from your browser" << std::endl
                  <<oauth.get_authorize_url() << std::endl;
        std::cout << "Please enter the code: ";
        std::string code;
        std::cin >> code;
        oauth.build_credential(code, cred);
    }

    Drive service(&cred);
    std::vector<GChange> changes = service.changes().Listall();

    for (int i = 0; i < changes.size(); i ++ ) {
        std::cout << "Id " << changes[i].get_id() << std::endl
                  << "File id " << changes[i].get_fileId() << std::endl;
        if (changes[i].get_deleted() == true) {
            std::cout << "File already deleted" << std::endl;
        } else {
            GFile file = changes[i].get_file();
            std::cout << "File title " << file.get_title() << std::endl;
        }
    }
    changes.clear();

    ChangeListRequest list = service.changes().List();

    while(true) {
        GChangeList changelist = list.execute();
        const std::vector<GChange>& items = changelist.get_items();
        changes.insert(changes.end(), items.begin(), items.end());

        std::string pageToken = changelist.get_nextPageToken();
        if (pageToken == "") {
            break;
        } else {
            list.clear();
            list.set_pageToken(pageToken);
        }
    }

    for (int i = 0; i < changes.size(); i ++ ) {
        std::cout << "Id " << changes[i].get_id() << std::endl
                  << "File id " << changes[i].get_fileId() << std::endl;
        if (changes[i].get_deleted() == true) {
            std::cout << "File already deleted" << std::endl;
        } else {
            GFile file = changes[i].get_file();
            std::cout << "File title " << file.get_title() << std::endl;
        }
    }
    changes.clear();

}
