#ifndef __GDRIVE_PARENTSERVICE_HPP__
#define __GDRIVE_PARENTSERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "common/all.hpp"

#include <vector>

namespace GDRIVE {

class ParentService {
    CLASS_MAKE_LOGGER
    public:
        static ParentService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }
        
        ParentListRequest List(std::string file_id);
        ParentGetRequest Get(std::string file_id, std::string parent_id);
        ParentInsertRequest Insert(std::string file_id, GParent* parent);
        ParentDeleteRequest Delete(std::string file_id, std::string parent_id);
    private:
        ParentService();
        ParentService(const ParentService& other);
        ParentService& operator=(const ParentService& other);

        static ParentService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }

};
}
#endif
