#ifndef __GDRIVE_DRIVE_HPP__
#define __GDRIVE_DRIVE_HPP__

#include "gdrive/util.hpp"
#include "gdrive/credential.hpp"
#include "gdrive/gitem.hpp"
#include "gdrive/config.hpp"
#include "gdrive/service/files.hpp"
#include "gdrive/service/about.hpp"
#include "gdrive/service/changes.hpp"
#include "gdrive/service/children.hpp"
#include "gdrive/service/parents.hpp"
#include "gdrive/service/permissions.hpp"
#include "gdrive/service/revisions.hpp"
#include "gdrive/service/apps.hpp"
#include "gdrive/service/replies.hpp"
#include "gdrive/service/comments.hpp"
#include "common/all.hpp"

#include <vector>


namespace GDRIVE {

class Drive {
    CLASS_MAKE_LOGGER
    public:
        Drive(Credential* cred);
        FileService& files();
        AboutService& about();
        ChangeService& changes();
        ChildrenService& children();
        ParentService& parents();
        PermissionService& permissions();
        RevisionService& revisions();
        AppService& apps();
        ReplyService& replies();
        CommentService& comments();
    protected:
        Credential* _cred;
};


}

#endif
