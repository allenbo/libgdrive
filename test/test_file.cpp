#include "gdrive/oauth.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/drive.hpp"
#include "gdrive/servicerequest.hpp"
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
    /*
    std::vector<GFile> files = service.files().List().execute();
    for (int i = 0; i < files.size(); i ++ ) {
        if (files[i].get_title() == "A new tile") {
            std::cout <<  "Find the file we need " << files[i].get_id() << std::endl;
            GFile file;
            file.set_title("Copy from file");
            file = service.files().Copy(files[i].get_id(), file).execute();
            break;
        }
    }
    */
    //service.files().EmptyTrash().execute();
    //
    // insert
    /*
    std::ifstream fin("document.txt");
    assert(fin.is_open());
    fin.seekg(0, std::ios::end);
    long file_size = fin.tellg();
    std::cout << "The size of this file is " << file_size << std::endl;
    fin.seekg(0, std::ios::beg);
    FileContent fc(fin, "text/plain");
    GFile file; 
    service.files().Insert(&file, &fc).execute();
    */
    /*
    std::ifstream fin("document.txt");
    assert(fin.is_open());
    FileContent fc(fin, "text/plain");
    GFile file;
    file.set_title("document");
    file.set_description("This is a test document");
    file.set_mimeType("text/plain");
    service.files().Insert(&file, &fc).execute();
    fin.close();
    */
    /*
    std::ifstream fin("puppy.jpg", std::ios::binary);
    assert(fin.is_open());
    FileContent fc(fin, "image/jpeg");
    GFile file;
    file.set_title("puppy");
    file.set_description("This is a test image");
    file.set_mimeType("image/jpeg");
    service.files().Insert(&file, &fc).execute();
    fin.close();
    */
    /*
    std::ifstream fin("Silverlight.dmg", std::ios::binary);
    assert(fin.is_open());
    std::string mime = "application/x-apple-diskimage";
    FileContent fc(fin, mime);
    GFile file;
    file.set_title("SilverLight");
    file.set_mimeType(mime);
    service.files().Insert(&file, &fc).execute();
    fin.close();
    */
    /*
    std::ifstream fin("storageArrangement.pptx", std::ios::binary);
    assert(fin.is_open());

    std::string mime = "application/vnd.ms-powerpoint";
    FileContent fc(fin, mime);
    GFile file;
    file.set_title("storage arrangement");
    file.set_mimeType(mime);
    service.files().Insert(&file, &fc, true).execute();
    fin.close();
    */
    FileListRequest list = service.files().List();
    while (true) {
        list.set_maxResults(88);
        GFileList filelist = list.execute();
        list.clear();
        if (filelist.get_nextPageToken() != "") {
            list.set_pageToken(filelist.get_nextPageToken());
        } else {
            break;
        }
    }
    /*
    std::string file_id = "";
    for (int i = 0; i < files.size(); i ++ ) {
        if (files[i].get_title() == "storage arrangement") {
            std::cout <<  "Find the file we need " << files[i].get_id() << std::endl;
            file_id = files[i].get_id();
            break;
        }
    }
    
    std::ifstream fin("Silverlight.dmg", std::ios::binary);
    assert(fin.is_open());
    std::string mime = "applicaton/x-apple-diskimage";
    FileContent fc(fin, mime);

    GFile file = service.files().Get(file_id).execute();
    file.set_mimeType(mime);
    file.set_title("Another Silverlight");
    service.files().Update(file_id, &file, &fc).execute();
    */
}
