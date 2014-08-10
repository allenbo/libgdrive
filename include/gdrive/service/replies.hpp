#ifndef __GDRIVE_REPLYSERVICE_HPP__
#define __GDRIVE_REPLYSERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "common/all.hpp"

#include <vector>

namespace GDRIVE {

class ReplyService {
    CLASS_MAKE_LOGGER
    public:
        static ReplyService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }
        
        ReplyListRequest List(std::string file_id, std::string comment_id);
        ReplyGetRequest Get(std::string file_id, std::string comment_id, std::string reply_id);
        ReplyInsertRequest Insert(std::string file_id, std::string comment_id, GReply* reply);
        ReplyDeleteRequest Delete(std::string file_id, std::string comment_id, std::string reply_id);
        ReplyPatchRequest Patch(std::string file_id, std::string comment_id, std::string reply_id, GReply* reply);
        ReplyUpdateRequest Update(std::string file_id, std::string comment_id, std::string reply_id, GReply* reply);
    private:
        ReplyService();
        ReplyService(const ReplyService& other);
        ReplyService& operator=(const ReplyService& other);

        static ReplyService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }

};
}
#endif
