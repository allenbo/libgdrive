#include "gdrive/fileservice.hpp"
#include "jconer/json.hpp"

#include <string.h>
using namespace JCONER;

namespace GDRIVE {

FileService FileService::_single_instance;

FileService::FileService()
{
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("FileService", L_DEBUG)
#endif
}

FileListRequest FileService::List() {
    FileListRequest flr(_cred, FILES_URL);
    return flr;
}

std::vector<GFile> FileService::Listall() {
    FileListRequest list = List();
    std::vector<GFile> files;
    while(true) {
        GFileList filelist = list.execute();
        const std::vector<GFile>& tmp = filelist.get_items();
        files.insert(files.end(), tmp.begin(), tmp.end());
        list.clear();

        std::string pageToken = filelist.get_nextPageToken();
        if (pageToken == "") {
            break;
        } else {
            list.set_pageToken(pageToken);
        }
    }
    return files;
}

FileGetRequest FileService::Get(std::string id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(id);
    FileGetRequest fgr(_cred, vs.toString());
    return fgr;
}

FileTrashRequest FileService::Trash(std::string id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(id).append("/trash");
    FileTrashRequest request(_cred, vs.toString());
    return request;
}

FileUntrashRequest FileService::Untrash(std::string id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(id).append("/untrash");
    FileUntrashRequest request(_cred, vs.toString());
    return request;
}

FileDeleteRequest FileService::Delete(std::string id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(id);
    FileDeleteRequest request(_cred, vs.toString());
    return request;
}

FileEmptyTrashRequest FileService::EmptyTrash() {
    VarString vs;
    vs.append(FILES_URL).append("/trash");
    FileEmptyTrashRequest request(_cred, vs.toString());
    return request;
}

FileTouchRequest FileService::Touch(std::string id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(id).append("/touch");
    FileTouchRequest request(_cred, vs.toString());
    return request;
}

FilePatchRequest FileService::Patch(std::string file_id, GFile* file) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id);
    FilePatchRequest pr(file, _cred, vs.toString());
    return pr;
}

FileCopyRequest FileService::Copy(std::string file_id, GFile* file) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/copy");
    FileCopyRequest fcr(file, _cred, vs.toString());
    return fcr;
}

FileInsertRequest FileService::Insert(GFile* file, FileContent* content, bool resumable) {
    VarString vs;
    vs.append(FILE_UPLOAD_URL);
    FileInsertRequest fir(content, file, _cred, vs.toString(), resumable);
    return fir;
}

FileUpdateRequest FileService::Update(std::string id, GFile* file, FileContent* content, bool resumable) {
    VarString vs;
    vs.append(FILE_UPLOAD_URL).append("/").append(id);
    FileUpdateRequest fur(content, file, _cred, vs.toString(), resumable);
    return fur;
}

}
