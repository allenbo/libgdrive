#include "gdrive/store.hpp"
#include <stdlib.h>
#include <stdio.h>
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
    std::string client_id = "client_id_with_random_char_cksjflaueklajfdal;s";
    std::string client_secret = "client_secret_with_random_char_dajla09alksfhfjalajsdfl";
    fs.put("client_id", client_id);
    fs.put("client_secret", client_secret);
    fs.dump();

    FileStore tester(filename);
    assert(tester.status() == SS_FULL);
    assert(fs.get("client_id") == client_id);
    assert(fs.get("client_secret") == client_secret);

    if (remove(filename.c_str()) != 0) {
        std::cerr << "Can't remove the file " << filename
                  << "Please remove it manually" << std::endl;
    }
}
