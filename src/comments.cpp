#include "gdrive/service/comments.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

CommentService CommentService::_single_instance;

CommentService::CommentService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("CommentService", L_DEBUG)
#endif
}

CommentListRequest CommentService::List(std::string file_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments");
    CommentListRequest r(_cred, vs.toString());
    return r;
}

CommentGetRequest CommentService::Get(std::string file_id, std::string comment_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id);
    CommentGetRequest r(_cred, vs.toString());
    return r;
}

CommentInsertRequest CommentService::Insert(std::string file_id, GComment* comment) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments");
    CommentInsertRequest r(comment, _cred, vs.toString());
    return r;
}

CommentDeleteRequest CommentService::Delete(std::string file_id, std::string comment_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id);
    CommentDeleteRequest r(_cred, vs.toString());
    return r;
}

CommentPatchRequest CommentService::Patch(std::string file_id, std::string comment_id, GComment* comment) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id);
    CommentPatchRequest r(comment, _cred, vs.toString());
    return r;
}

CommentUpdateRequest CommentService::Update(std::string file_id, std::string comment_id, GComment* comment) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id);
    CommentUpdateRequest r(comment, _cred, vs.toString());
    return r;
}

}
