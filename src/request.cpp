#include "gdrive/request.hpp"
#include <curl.h>

namespace GDRIVE {

Request::Request(std::string uri, RequestMethod method)
    :_uri(uri), _method(method) 
{
    _init_curl_handle();    
}

Request::Request(std::string uri, RequestMethod method, RequestBody& body, RequestHeader& header)
    :_uri(uri), _method(method)
{
    _init_curl_handle();
    _body.insert(body.begin(), body.end());
    _header.insert(header.begin(), header.end());
}

Request::~Request() {
    curl_easy_cleanup(_handle);
}

void Request::_init_curl_handle() {
    _handle = curl_easy_init();
    curl_easy_setopt(_handle, CURL_URL, _uri.c_str());
}

curl_slist* Request::_build_header() {
    curl_slist* list = NULL;
    VarString vs;
    for(RequestHeader::iterator iter = _header.begin(); iter != _header.end(); iter ++) {
        list = curl_slist_append(list, vs.append(iter->key).append(':').append(iter->value).toString().c_str());
        vs.clear();
    }
    return list;
}

std::string Request::_build_body() {
    VarString vs;
    for(Request::iterator iter = _body.begin(); iter != _body.end(); iter ++) {
        vs.append(iter->key).append('=').append(URLHelper::encode(iter->second)).append('&');
    }
    return vs.drop().toString();
}

Response Request::get_response() {
    if (_body.size() > 0) {
        std::string encoded_body = _build_body();
        curl_easy_setopt(_handle, CURLOPT_POSTFIELDS, encoded_body.c_str());
    }
    curl_slist* header_list = NULL;
    if (_header.size() > 0) {
        header_list = _build_header();     
        curl_easy_setopt(_handle, CURLOPT_HTTPHEADER, header_list);
    }
    int rst = cur_easy_perform(_handle);
    
    if (header_list != NULL) {
        curl_slist_free_all(header_list);
    }
    return rst;
}



}
