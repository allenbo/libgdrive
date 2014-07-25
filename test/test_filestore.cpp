#include "gdrive/store.hpp"
#include <stdlib.h>
#include <cassert>
#include <iostream>
using namespace GDRIVE;

int main(int argc, char** argv) {
    if (argc != 2){
        std::cerr << "Usage: ./proj store_filename" << std::endl;
        exit(-1);
    }
    std::string filename(argv[1]);
    FileStore fs(filename);
    assert(fs.status() == SS_FULL);
    std::cout << fs.get("client_id") << std::endl;
    std::cout << fs.get("client_secret") << std::endl;

    fs.put("refresh_token", "some_long_string");
    fs.dump();
}
