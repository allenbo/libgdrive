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
    GAbout about = service.about().Get().execute();
    std::cout << "Current user name " << about.get_name() << std::endl
              << "Root folder id " << about.get_rootFolderId() << std::endl
              << "Total quota (bytes) " << about.get_quotaBytesTotal() << std::endl
              << "Used quota (bytes) " << about.get_quotaBytesUsed() << std::endl;
}
