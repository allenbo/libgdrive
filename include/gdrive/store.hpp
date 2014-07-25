#ifndef __GDRIVE_STORE_HPP__
#define __GDRIVE_STORE_HPP__

#include "gdrive/util.hpp"
#include "gdrive/logging.hpp"

#include <map>
#include <fstream>

namespace GDRIVE {

enum StoreStatus {
    SS_EMPTY = 0,
    SS_FULL
};

class Store {
    public:
        virtual std::string get(std::string key) = 0;
        virtual void put(std::string key, std::string value) = 0;
        virtual bool dump() = 0;
        inline StoreStatus status() const { return _status; }
    protected:
        StoreStatus _status;
};

class FileStore : public Store {
    CLASS_MAKE_LOGGER
    public:
        FileStore(std::string filename);
        std::string get(std::string);
        void put(std::string key, std::string value);
        bool dump();
    private:
        std::map<std::string, std::string>  _content;
        std::string _filename;
};


}

#endif
