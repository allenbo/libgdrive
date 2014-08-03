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
            _length = -1;
            _resumable_cur_pos = _resumable_start_pos = _resumable_length = 0;
#ifdef GDRIVE_DEBUG
            CLASS_INIT_LOGGER("FileContent", L_DEBUG)
#endif
        }

        FileContent(const FileContent& other)
            :_fin(other._fin), _mimetype(other._mimetype)
        {
            _length = other._length;
            _resumable_length = other._resumable_length;
            _resumable_start_pos = other._resumable_start_pos;
            _resumable_cur_pos = other._resumable_cur_pos;
#ifdef GDRIVE_DEBUG
            CLASS_INIT_LOGGER("FileContent", L_DEBUG)
#endif
        }
 
        int get_length();
        inline std::string mimetype() const { return _mimetype; }

        std::string get_content();
        static size_t read(void* ptr, size_t size, size_t nmemb, void* userp);
        static size_t resumable_read(void* ptr, size_t size, size_t nmemb, void* userp);

        void set_resumable_start_pos(int pos);
        void set_resumable_length(int length);
    protected:
        int _getRemainingLength();
        std::ifstream& _fin;
        std::string _mimetype;
        int _length;
 
        int _resumable_start_pos;
        int _resumable_length;
        int _resumable_cur_pos;
};

}

#endif
