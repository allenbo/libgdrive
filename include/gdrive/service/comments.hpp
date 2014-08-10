#ifndef __GDRIVE_COMMENTSERVICE_HPP__
#define __GDRIVE_COMMENTSERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "common/all.hpp"

#include <vector>

namespace GDRIVE {

class CommentService {
    CLASS_MAKE_LOGGER
    public:
        static CommentService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }
        
        CommentListRequest List(std::string file_id);
        CommentGetRequest Get(std::string file_id, std::string comment_id);
        CommentInsertRequest Insert(std::string file_id, GComment* comment);
        CommentDeleteRequest Delete(std::string file_id, std::string comment_id);
        CommentPatchRequest Patch(std::string file_id, std::string comment_id, GComment* comment);
        CommentUpdateRequest Update(std::string file_id, std::string comment_id, GComment* comment);
    private:
        CommentService();
        CommentService(const CommentService& other);
        CommentService& operator=(const CommentService& other);

        static CommentService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }

};
}
#endif
