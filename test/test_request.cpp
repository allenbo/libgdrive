#include "gdrive/request.hpp"

using namespace GDRIVE;

int main() {
    RequestBody body;
    body["name"] = "joe";
    body["comment"] = "A test comment";

    RequestHeader header;
    Request request("http://bitworking.org/news/223/Meet-Ares", RM_POST, body, header);
    request.get_response();
}
