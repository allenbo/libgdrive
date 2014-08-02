#ifndef __GDRIVE_FILECONTENT_HPP__
#define __GDRIVE_FILECONTENT_HPP__

#include "gdrive/util.hpp"
#include "gdrive/config.hpp"
#include "gdrive/logging.hpp"

#include <fstream>

namespace GDRIVE {

class FileContent {
    CLASS_MAKE_LOGGER
    public:
        FileContent(std::ifstream& fin, std::string mimetype)
            :_fin(fin), _mimetype(mimetype)
        {
#ifdef GDRIVE_DEBUG
            CLASS_INIT_LOGGER("FileContent", L_DEBUG)
#endif
        }

        FileContent(const FileContent& other)
            :_fin(other._fin), _mimetype(other._mimetype)
        {
#ifdef GDRIVE_DEBUG
            CLASS_INIT_LOGGER("FileContent", L_DEBUG)
#endif
        }
 
        int get_length();
        inline std::string mimetype() const { return _mimetype; }

        std::string get_content();
        static size_t read(void* ptr, size_t size, size_t nmemb, void* userp);
    protected:
        int _getRemainingLength();
        std::ifstream& _fin;
        std::string _mimetype;
};

}

#endif
