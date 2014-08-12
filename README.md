gdrive
======

A C++ implementation of Google drive API.

## Build
Gdrive is using my JSON implementation [JConer](https://github.com/allenbo/JConer) and utility classes in [common](https://github.com/allenbo/common). You should also download and install them.

```
git clone https://github.com/allenbo/common.git
git clone https://github.com/allenbo/JConer.git
cd JConer && cp -R ../common/include/common include/ && make
cd ../
git clone https://github.com/allenbo/gdrive.git
cd gdrive 
cp -R ../JConer/include/jconer include/ && cp ../JConer/libjconer.a lib/
cp -R ../common/include/common include/
make
```

## Setup
Gdrive is an interface for C++ users to interact with Google drive. It doesn't provide any application with itself. In
order to use it, you should link the include director and libgdrive.a to your program before you compile it.

Gdrive also need a specific environ variable `GDRIVE` that link to a file with such structure
```
client_id=your_client_id
client_secret=your_client_secret
```
Please notice you can't put any space besides equal sign.

## Example
Since Google has already provided a Java SDK for drive, the interface Gdrive providing here is somehow like the Java version.
(Yes, I am trying to make everything just like the offical one.) Here is some examples how gdrive interact with Google drive.

**Get Credential**

```
#include "gdrive/gdrive.hpp"
using namespace GDRIVE;

int main() {
    char* gdrive_dir = getenv("GDRIVE");

    FileStore fs(gdrive_dir);
    Credential cred(&fs);

    if (cred.invalid() == true) {
        std::string client_id = fs.get("client_id");
        std::string client_secret = fs.get("client_secret");

        OAuth oauth(client_id, client_secret);
        std::cout << "Please go to this link in your browser, after you authorize this application, you will get a code from your browser" << std::endl
                  << oauth.get_authorize_url() << std::endl;
        std::cout << "Please enter the code: ";
        std::string code;
        std::cin >> code;
        oauth.build_credential(code, cred);
    }
}
```

Credential needs a file store to save the authorization code that gdrive fetched from google for sequal usage. With file store, authorization would
be one time operation. Otherwise, user has to authorize every time that he/she uses gdrive.

**File Operation**
Please check out Google drive [offical API documentation](https://developers.google.com/drive/v2/reference/) for details.

* **List files**
```
/*
   Get Credential
   */
Drive service(&cred);
std::vector<GFile> files = service.files().Listall();

// or
FileListRequest list = service.files().List();
std::vector<GFile> files;

while(true) {
    GFileList filelist = list.exeute();
    list.clear(); // you have to clear the response of previous request;
    std::string pageToken = filelist.get_nextPageToken();
    const std::vector<GFile>& tmp = filelist.get_items();
    files.insert(files.end(), tmp.begin(), tmp.end());
    if(pageToken == "") {
        break;
    } else {
        list.set_pageToken(pagToken);
    }
}
```

* **Get file**
```
GFile file = service.files().Get(file_id).execute();
```
* **Insert new file**
```
ifstream fin("some_image_file.jpg", std::ios::binary);
FileContent fc(fin, "image/jpeg");
GFile file;
file.set_title("Some Image File");
file.set_decription("This is a image file for test");
file.set_mimeType("image/jpeg");

service.files().Insert(&file, &fc).execute();
```
* **Patch file**
Patch operation would update the metadata of files in drive.
```
GFile file;
file.set_title("New Title");
file.set_decription("New Description");

FilePatchRequest patch = service.files().Patch(file_id, &file);
GFile updated_file = patch.execute();
```
For other operations, please check out include/gdrive/service/files.hpp for more information.

## Support
* All file operations except watch are covered
* About operations are all covered
* All change operations except watch are covered
* Child operations are all covered
* Parent operations are all covered
* Permission operations are all covered
* Revision operations are all covered
* App operation are all covered
* Comment operation are all covered
* Reply operation are all covered
