#include "gdrive/service/replies.hpp"
#include "jconer/json.hpp"

#include <string.h>
#include <vector>
using namespace JCONER;

namespace GDRIVE {

ReplyService ReplyService::_single_instance;

ReplyService::ReplyService() {
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("ReplyService", L_DEBUG)
#endif
}

ReplyListRequest ReplyService::List(std::string file_id, std::string comment_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/commments/").append(comment_id);
    ReplyListRequest r(_cred, vs.toString());
    return r;
}

ReplyGetRequest ReplyService::Get(std::string file_id, std::string comment_id, std::string reply_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id).append("/replies/").append(reply_id);
    ReplyGetRequest r(_cred, vs.toString());
    return r;
}

ReplyInsertRequest ReplyService::Insert(std::string file_id, std::string comment_id, GReply* reply) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id).append("/replies");
    ReplyInsertRequest r(reply, _cred, vs.toString());
    return r;
}

ReplyDeleteRequest ReplyService::Delete(std::string file_id, std::string comment_id, std::string reply_id) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id).append("/replies/").append(reply_id);
    ReplyDeleteRequest r(_cred, vs.toString());
    return r;
}

ReplyPatchRequest ReplyService::Patch(std::string file_id, std::string comment_id, std::string reply_id, GReply* reply) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id).append("/replies/").append(reply_id);
    ReplyPatchRequest r(reply, _cred, vs.toString());
    return r;
}

ReplyUpdateRequest ReplyService::Update(std::string file_id, std::string comment_id, std::string reply_id, GReply* reply) {
    VarString vs;
    vs.append(FILES_URL).append('/').append(file_id).append("/comments/").append(comment_id).append("/replies/").append(reply_id);
    ReplyUpdateRequest r(reply, _cred, vs.toString());
    return r;
}

}
