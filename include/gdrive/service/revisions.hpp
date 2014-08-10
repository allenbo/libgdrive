#ifndef __GDRIVE_REVISIONSERVICE_HPP__
#define __GDRIVE_REVISIONSERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "common/all.hpp"

#include <vector>

namespace GDRIVE {

class RevisionService {
    CLASS_MAKE_LOGGER
    public:
        static RevisionService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }
        
        RevisionListRequest List(std::string file_id);
        RevisionGetRequest Get(std::string file_id, std::string revision_id);
        RevisionDeleteRequest Delete(std::string file_id, std::string revision_id);
        RevisionPatchRequest Patch(std::string file_id, std::string revision_id, GRevision* revision);
        RevisionUpdateRequest Update(std::string file_id, std::string revision_id, GRevision* revision);
    private:
        RevisionService();
        RevisionService(const RevisionService& other);
        RevisionService& operator=(const RevisionService& other);

        static RevisionService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }

};
}
#endif
