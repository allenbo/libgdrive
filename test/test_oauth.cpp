#include "gdrive/oauth.hpp"
#include <iostream>

using namespace GDRIVE;

int main() {
    std::string client_id = "1012249028717-niumvjtkrrnmdj7g2ri1jadn7bg6mr91.apps.googleusercontent.com";
    std::string client_secret = "Z_MJN17w9Qi3aERlxq56tlyb";
    OAuth oauth(client_id, client_secret);    
    std::cout << "Please go to this url using your browser, after you authorize this application, you will get a code from your browser" << std::endl
              <<oauth.get_authorize_url() << std::endl;
    std::cout << "Please enter the code: ";
    std::string code;
    std::cin >> code;
    oauth.build_credential(code);
}


