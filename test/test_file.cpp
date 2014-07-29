#include "gdrive/oauth.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/drive.hpp"
#include <iostream>
#include <assert.h>

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

    Credential cred;

    if (fs.get("refresh_token") == "") {
        std::string client_id = fs.get("client_id");
        std::string client_secret = fs.get("client_secret");

        OAuth oauth(client_id, client_secret);    
        std::cout << "Please go to this url using your browser, after you authorize this application, you will get a code from your browser" << std::endl
                  <<oauth.get_authorize_url() << std::endl;
        std::cout << "Please enter the code: ";
        std::string code;
        std::cin >> code;
        cred = oauth.build_credential(code);
        cred.set_store(&fs);
    } else {
        cred.from_store(&fs);
    }
    Request request("https://www.googleapis.com/discovery/v1/apis/drive/v2/rest", RM_GET);
    Response resp = cred.request(request);
    std::cout << resp.status() << " " << resp.content() << std::endl;
    Drive service(cred);
    std::vector<GFile> files = service.files().List();
    for (int i = 0; i < files.size(); i ++ ) {
        if (files[i].title == "testforgdrive") {
            std::cout <<  "Find the file we need " << files[i].id << std::endl;
            GFile file = service.files().Touch(files[i].id);
            break;
        }
    }
}
