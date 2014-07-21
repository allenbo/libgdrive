#ifndef __GDRIVE_REQUEST_HPP__
#define __GDRIVE_REQUEST_HPP__

#include "logging.hpp"
#include <string>
#include <map>
#include <vector>
#include <curl/curl.h>

namespace GDRIVE {

enum RequestMethod {
    RM_GET,
    RM_POST,
    RM_PUT
};

typedef std::map<std::string, std::string> RequestHeader;
typedef std::map<std::string, std::string> RequestBody;
/*
class Response {
    CLASS_MAKE_LOGGER
    public:
        Response();
};
*/

typedef int Response;

class Request {
    CLASS_MAKE_LOGGER
    public:
        Request(std::string uri, RequestMethod method);
        Request(std::string uri, RequestMethod method, RequestBody& body, RequestHeader& header);
        void set_header(RequestHeader &header);
        void set_body(RequestBody &body);

        Response get_response();
        ~Request();
    private:
        std::string _uri;
        RequestMethod _method;
        RequestHeader _header;
        RequestBody _body;
        CURL *_handle;
        
        void _init_curl_handle();
        std::string _build_body();
        curl_slist* _build_header();
};

}

#endif
