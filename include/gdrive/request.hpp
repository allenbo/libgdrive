#ifndef __GDRIVE_REQUEST_HPP__
#define __GDRIVE_REQUEST_HPP__

#include "gdrive/logging.hpp"
#include "gdrive/config.hpp"
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

class Response;
class Request;

class Response {
    CLASS_MAKE_LOGGER
    public:
        Response() {}
        static size_t curl_write_callback(void* content, size_t size, size_t nmemb, void* userp);
        inline std::string content() const { return _content; };
        inline std::string header() const { return _header; };
        inline int status() const { return _status; }
        inline void set_status(int status) { _status = status;}
    private:
        std::string _content;
        std::string _header;
        int _status;

        friend class Request;
};


class Request {
    CLASS_MAKE_LOGGER
    public:
        Request(std::string uri, RequestMethod method);
        Request(std::string uri, RequestMethod method, RequestBody& body, RequestHeader& header);
        void set_header(RequestHeader &header);
        void set_body(RequestBody &body);
        void request();
        inline Response& response() { return _resp;}
        ~Request();
    private:
        std::string _uri;
        RequestMethod _method;
        RequestHeader _header;
        RequestBody _body;
        Response _resp;
        CURL *_handle;
        
        void _init_curl_handle();
        std::string _build_body();
        curl_slist* _build_header();
};

}

#endif
