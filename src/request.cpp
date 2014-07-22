#include "gdrive/request.hpp"
#include "gdrive/util.hpp"
#include "gdrive/config.hpp"
#include <curl/curl.h>

namespace GDRIVE {

Request::Request(std::string uri, RequestMethod method)
    :_uri(uri), _method(method) 
{
    _init_curl_handle();    
#ifdef DEBUG
    CLASS_INIT_LOGGER("Request", L_DEBUG);
#endif
}

Request::Request(std::string uri, RequestMethod method, RequestBody& body, RequestHeader& header)
    :_uri(uri), _method(method)
{
    _init_curl_handle();
    _body.insert(body.begin(), body.end());
    _header.insert(header.begin(), header.end());
#ifdef DEBUG
    CLASS_INIT_LOGGER("Request", L_DEBUG);
#endif
}

Request::~Request() {
    curl_easy_cleanup(_handle);
}

void Request::_init_curl_handle() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    _handle = curl_easy_init();
    curl_easy_setopt(_handle, CURLOPT_URL, _uri.c_str());
}

curl_slist* Request::_build_header() {
    curl_slist* list = NULL;
    VarString vs;
    for(RequestHeader::iterator iter = _header.begin(); iter != _header.end(); iter ++) {
        list = curl_slist_append(list, vs.append(iter->first).append(':').append(iter->second).toString().c_str());
        vs.clear();
    }
    return list;
}

std::string Request::_build_body() {
    VarString vs;
    for(RequestBody::iterator iter = _body.begin(); iter != _body.end(); iter ++) {
        vs.append(iter->first).append('=').append(URLHelper::encode(iter->second)).append('&');
    }
    return vs.drop().toString();
}

#ifdef DEBUG
Response Request::get_response(const char* msg) {
    if (msg != NULL) {
        curl_easy_setopt(_handle, CURLOPT_POSTFIELDS, msg);
    } else {
#else
Response Request::get_response() {
#endif
    if (_body.size() > 0) {
        std::string encoded_body = _build_body();
        if (_method == RM_POST) {
            //CLOG_DEBUG("Generate encoded body:%s\n", encoded_body.c_str());
            //curl_easy_setopt(_handle, CURLOPT_POST, 1);
            char* message = NULL;
            MALLOC(char, message, encoded_body.size() + 1);
            strcpy(message, encoded_body.c_str());
            CLOG_DEBUG("Generate encoded body:%s\n", message);
            curl_easy_setopt(_handle, CURLOPT_POSTFIELDS, message);
            //curl_easy_setopt(_handle, CURLOPT_POSTFIELDSIZE, encoded_body.size());
        } else if (_method == RM_GET) {
            VarString vs;
            vs.append(_uri).append('&').append(encoded_body);
            curl_easy_setopt(_handle, CURLOPT_URL, vs.toString().c_str());
        }
    }
#ifdef DEBUG
    }
#endif
    curl_easy_setopt(_handle, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(_handle, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(_handle, CURLOPT_HEADER, 1);
    curl_slist* header_list = NULL;
    if (_header.size() > 0) {
        header_list = _build_header();
        curl_easy_setopt(_handle, CURLOPT_HTTPHEADER, header_list);
    }
    int rst = curl_easy_perform(_handle);
    
    if (header_list != NULL) {
        curl_slist_free_all(header_list);
    }
    return rst;
}



}
