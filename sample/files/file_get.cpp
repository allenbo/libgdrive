#include "gdrive/gdrive.hpp"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>

using namespace GDRIVE;

GFile get_file(Drive& service, std::string file_id) {
    GFile file = service.files().Get(file_id).execute();
    std::cout << "Title " << file.get_title() << std::endl
              << "Decription " << file.get_description() << std::endl
              << "DownloadUrl " << file.get_downloadUrl() << std::endl
              << "MIME type " << file.get_mimeType() << std::endl;
    return file;
}

void download_file(GFile& file, Credential* cred) {
    std::string url = file.get_downloadUrl();
    if (url == "") {
        url = file.get_exportLinks()["application/pdf"];
    }
    CredentialHttpRequest request(cred, url, RM_GET);
    HttpResponse resp = request.request();
    std::string filename = file.get_originalFilename();
    if (filename == "") {
        filename = file.get_title();
    }

    std::ofstream fout(filename.c_str(), std::ios::binary);
    fout.write(resp.content().c_str(), resp.content().size());
    fout.close();
}

void usage() {
    std::cerr << "./file_get [file_id]" << std::endl;
    exit(-1);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        usage();
    }
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

    GFile file = get_file(service, argv[1]);
    download_file(file, &cred);
}
