#include "gdrive/request.hpp"
#include "gdrive/util.hpp"
#include <iostream>

using namespace GDRIVE;

int main() {
    std::map<std::string, std::string> body;
    body["name"] = "joe";
    body["comment"] = "A test comment";

    RequestHeader header;
    HttpRequest request("http://bitworking.org/news/223/Meet-Ares", RM_POST, header, URLHelper::encode(body));
    HttpResponse resp = request.request();
    std::cout << resp.content() << std::endl;
    std::cout << resp.header() << std::endl;
    std::cout << resp.status() << std::endl;
}
