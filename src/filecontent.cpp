#include "gdrive/filecontent.hpp"

namespace GDRIVE  {

int FileContent::get_length() {
    if (_length == -1) {
        long cur_pos = _fin.tellg();
        _fin.seekg(0, std::ios::end);
        _length = _fin.tellg();
        _fin.seekg(cur_pos, std::ios::beg);
    }
    return _length;
}

int FileContent::_getRemainingLength() {
    long cur_pos = _fin.tellg();
    return get_length() - cur_pos;
}

std::string FileContent::get_content() {
    long cur_pos = _fin.tellg();
    int filesize = get_length();
    if (filesize < 0) {
        CLOG_FATAL("File size is less than 0: [%d]\n", filesize);
    }
    char* buffer = new char[filesize];
    _fin.seekg(0, std::ios::beg);
    _fin.read(buffer, filesize);
    std::string rst(buffer, filesize);
    delete [] buffer;
    _fin.seekg(cur_pos, std::ios::beg);
    return rst;
}

size_t FileContent::read(void* ptr, size_t size, size_t nmemb, void* userp) {
    FUNC_MAKE_LOGGER
    FUNC_LOGGER_SET_LEVEL(COMMON::L_DEBUG);

    FileContent* fc = (FileContent*)userp;

    int remaining_length = fc->_getRemainingLength();
    if (remaining_length == 0) {
        fc->_fin.seekg(0, std::ios::beg);
        return 0;
    }

    int length = size * nmemb > remaining_length ? remaining_length : size * nmemb;
    fc->_fin.read((char*)ptr, length);
    FLOG_DEBUG("Read %d from filecontent\n", length);
    return length;
}

size_t FileContent::resumable_read(void *ptr, size_t size, size_t nmemb, void* userp) {
    FUNC_MAKE_LOGGER
    FUNC_LOGGER_SET_LEVEL(COMMON::L_DEBUG);

    FileContent* fc = (FileContent*)userp;

    int remaining = fc->_resumable_length - (fc->_resumable_cur_pos - fc->_resumable_start_pos);
    if (remaining == 0) {
        return 0;
    }

    int length = size * nmemb > remaining ? remaining : size * nmemb;
    fc->_fin.read((char*)ptr, length);
    FLOG_DEBUG("Read %d from filecontent\n", length);
    fc->_resumable_cur_pos += length;
    return length;
}

void FileContent::set_resumable_start_pos(int pos) {
    if (pos < 0 || pos > get_length()) {
        CLOG_FATAL("Error start pos for resumable: %d\n", pos);
    }
    _resumable_start_pos = pos;
    _fin.seekg(pos, std::ios::beg);
}

void FileContent::set_resumable_length(int length) {
    if (length <= 0 || length + _resumable_start_pos > get_length()) {
        CLOG_FATAL("Error length for resumable: %d\n", length);
    }
    _resumable_length = length;
}

}
