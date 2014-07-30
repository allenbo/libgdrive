#include "gdrive/request.hpp"
#include "gdrive/util.hpp"
#include "gdrive/config.hpp"
#include <curl/curl.h>

namespace GDRIVE {

size_t HttpResponse::curl_write_callback(void* content, size_t size, size_t nmemb, void* userp) {
    std::string* self = (std::string*)userp;
    std::string curr_content((char*)content, size * nmemb);
    *self += curr_content;
    return size * nmemb;
}

HttpRequest::HttpRequest(std::string uri, RequestMethod method)
    :_uri(uri), _method(method) 
{
    _init_curl_handle();    
#ifdef GDIRVE_DEBUG
    CLASS_INIT_LOGGER("HttpRequest", L_DEBUG);
#endif
}

HttpRequest::HttpRequest(std::string uri, RequestMethod method, RequestHeader& header, std::string body)
    :_uri(uri), _method(method), _body(body)
{
    _init_curl_handle();
    _header.insert(header.begin(), header.end());
#ifdef GDRIVE_DEBUG
    CLASS_INIT_LOGGER("HttpRequest", L_DEBUG);
#endif
}

HttpRequest::~HttpRequest() {
    curl_easy_cleanup(_handle);
}

void HttpRequest::set_uri(std::string uri) {
    _uri = uri;
    curl_easy_setopt(_handle, CURLOPT_URL, _uri.c_str());
}

void HttpRequest::_init_curl_handle() {
    //curl_global_init(CURL_GLOBAL_ALL);
    _handle = curl_easy_init();
    curl_easy_setopt(_handle, CURLOPT_URL, _uri.c_str());
    curl_easy_setopt(_handle, CURLOPT_HEADERDATA, (void*)&_resp._header);
    curl_easy_setopt(_handle, CURLOPT_WRITEDATA, (void*)&_resp._content);
    curl_easy_setopt(_handle, CURLOPT_WRITEFUNCTION, HttpResponse::curl_write_callback);
    _header.clear();
    _query.clear();
}

void HttpRequest::add_header(RequestHeader& header) {
    _header.insert(header.begin(), header.end());
}

void HttpRequest::add_header(std::string key, std::string value) {
    _header[key] = value;
}

void HttpRequest::add_query(RequestQuery& query) {
    _query.insert(query.begin(), query.end());
}

void HttpRequest::add_query(std::string key, std::string value) {
    _query[key] = value;
}

curl_slist* HttpRequest::_build_header() {
    curl_slist* list = NULL;
    VarString vs;
    for(RequestHeader::iterator iter = _header.begin(); iter != _header.end(); iter ++) {
        list = curl_slist_append(list, vs.append(iter->first).append(':').append(iter->second).toString().c_str());
        vs.clear();
    }
    return list;
}

HttpResponse& HttpRequest::request() {
    VarString vs;
    // if there is query paremeter, append to url
    if (_query.size() != 0) {
        vs.append(_uri).append('?').append(URLHelper::encode(_query));
        curl_easy_setopt(_handle, CURLOPT_URL, vs.toString().c_str());
    }
    
    if (_method == RM_GET) {
        // do nothing
    } else {
        curl_easy_setopt(_handle, CURLOPT_POSTFIELDS, _body.c_str());
        if (_method == RM_POST) {
            if (_body == "") {
                _header["Content-Type"] = "";
            }
        } else if (_method == RM_DELETE) {
            curl_easy_setopt(_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
        } else {
            CLOG_FATAL("Unknown  method\n");
        }
    }
#ifdef GDRIVE_DEBUG
    curl_easy_setopt(_handle, CURLOPT_VERBOSE, 1);
#endif
    curl_easy_setopt(_handle, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_slist* header_list = NULL;
    if (_header.size() > 0) {
        header_list = _build_header();
        curl_easy_setopt(_handle, CURLOPT_HTTPHEADER, header_list);
    }
    CURLcode res = curl_easy_perform(_handle);
    
    if (header_list != NULL) {
        curl_slist_free_all(header_list);
    }

    if (res != CURLE_OK) {
        CLOG_ERROR("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    int status;
    curl_easy_getinfo(_handle, CURLINFO_RESPONSE_CODE, &status);
 
    _resp.set_status(status);
    return _resp;
}

}
