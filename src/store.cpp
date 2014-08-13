#include "gdrive/store.hpp"

namespace GDRIVE {

FileStore::FileStore(std::string filename)
    :_filename(filename)
{
    _status = SS_EMPTY;
    std::ifstream fin(_filename.c_str());
    if (!fin.good()){
        return;
    }

    int cur_pos = fin.tellg();
    fin.seekg(0, std::ios::end);
    int filesize = fin.tellg();
    fin.seekg(cur_pos, std::ios::beg);

    if (filesize == 0) {
        return;
    }

    while( true) {
        std::string line;
        getline(fin, line);
        if (fin.good() && line.size() != 0) {
            int pos = line.find('=');
            std::string key(line, 0, pos);
            std::string value(line, pos + 1);
            _content[key] = value;
            _status = SS_FULL;
        } else {
            break;
        }
    }
}

std::string FileStore::get(std::string key) {
    if (_content.find(key) != _content.end()) {
        return _content[key];
    }
    return "";
}

void FileStore::put(std::string key, std::string value) {
    _content[key] = value;
}

bool FileStore::dump() {
    std::ofstream fout(_filename.c_str());
    if (!fout.good()) {
        return false;
    }

    for(std::map<std::string, std::string>::iterator iter = _content.begin();
            iter != _content.end(); iter ++) {
        fout << iter->first << "=" << iter->second << std::endl;
    }
    return true;
}

}
