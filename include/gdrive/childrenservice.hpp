#ifndef __GDRIVE_CHILDRENSERVICE_HPP__
#define __GDRIVE_CHILDRENSERVICE_HPP__

#include "gdrive/config.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/util.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/servicerequest.hpp"
#include "common/all.hpp"

#include <vector>

namespace GDRIVE {

class ChildrenService {
    CLASS_MAKE_LOGGER
    public:
        static ChildrenService& get_instance(Credential *cred) {
            _single_instance.set_cred(cred);
            return _single_instance;
        }

        ChildrenListRequest List(std::string folder_id);
        std::vector<GChildren> Listall(std::string folder_id);
    private:
        ChildrenService();
        ChildrenService(const ChildrenService& other);
        ChildrenService& operator=(const ChildrenService& other);

        static ChildrenService _single_instance;

        Credential* _cred;
        inline void set_cred(Credential* cred) {
            _cred = cred;
        }
};
}
#endif
