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
    RM_PUT,
    RM_DELETE,
    RM_PATCH
};

enum EncodeMethod {
    EM_URL,
    EM_JSON,
};

typedef std::map<std::string, std::string> RequestHeader;
typedef std::map<std::string, std::string> RequestQuery;
typedef size_t (*ReadFunction) (void*, size_t, size_t, void*);

class HttpResponse;
class HttpRequest;

class MemoryString {
    public:
        MemoryString(const char* str, int size)
            :_str(str), _size(size), _pos(0) {}
        static size_t read(void* ptr, size_t size, size_t nmemb, void* userp) {
            MemoryString* self = (MemoryString*)userp;
            if (self->_size - self->_pos == 0) return 0;
            int length = self->_size - self->_pos > size * nmemb ? size * nmemb : self->_size - self->_pos;
            memcpy(ptr, self->_str + self->_pos, length);
            self->_pos += length;
            return length;
        }
    private:
        const char*  _str;
        int _size;
        int _pos;
};

class HttpResponse {
    CLASS_MAKE_LOGGER
    public:
        HttpResponse() { _header_map.clear(); }
        static size_t curl_write_callback(void* content, size_t size, size_t nmemb, void* userp);
        inline std::string content() const { return _content; };
        inline std::string header() const { return _header; };
        inline void clear() { _content = ""; _header = ""; _header_map.clear(); }
        inline int status() const { return _status; }
        inline void set_status(int status) { _status = status;}

        std::string get_header(std::string field);
        void _parse_header();
    private:
        std::string _content;
        std::string _header;
        int _status;
        std::map<std::string, std::string> _header_map;

        friend class HttpRequest;
};


class HttpRequest {
    CLASS_MAKE_LOGGER
    public:
        HttpRequest(std::string uri, RequestMethod method);
        HttpRequest(std::string uri, RequestMethod method, RequestHeader& header, std::string body);
        void add_header(RequestHeader &header);
        void add_header(std::string key, std::string value);
        void add_query(RequestQuery& query);
        void add_query(std::string key, std::string value);
        inline void clear_header() { _header.clear();}
        inline void clear_query() { _query.clear(); }
        void clear();
        void set_uri(std::string uri);
        HttpResponse& request();
        inline HttpResponse& response() { return _resp;}
        ~HttpRequest();
    protected:
        std::string _uri;
        RequestMethod _method;
        RequestHeader _header;
        RequestQuery _query;
        std::string _body;
        HttpResponse _resp;
        CURL *_handle;
        ReadFunction _read_hook;
        void* _read_context;
        void _init_curl_handle();
        curl_slist* _build_header();
};

}

#endif
