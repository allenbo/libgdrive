#include "gdrive/request.hpp"
#include <iostream>

using namespace GDRIVE;

int main() {
    RequestBody body;
    body["name"] = "joe";
    body["comment"] = "A test comment";

    RequestHeader header;
    Request request("http://bitworking.org/news/223/Meet-Ares", RM_POST, body, header);
    request.request();
    Response resp = request.response();
    std::cout << resp.content() << std::endl;
    std::cout << resp.header() << std::endl;
    std::cout << resp.status() << std::endl;
}
