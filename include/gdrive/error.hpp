#ifndef __GDRIVE_ERROR_HPP__
#define __GDRIVE_ERROR_HPP__

#include <vector>
#include <map>
#include <exception>

#include "gdrive/gitem.hpp"

namespace GDRIVE {

class GoogleJsonResponseException : public std::exception {
    public:
        GoogleJsonResponseException(GError error)
            :_details(error)
        {
        }

        GError& details() {
            return _details;
        }

        virtual ~GoogleJsonResponseException() throw() {}

    private:
        GError _details;
};


class CurlException : public std::exception {
    public:
        CurlException(int code, std::string error)
            :_code(code), _error(error) {}
        std::string error() { return _error; }
        int code() { return _code;}
        virtual ~CurlException() throw() {}
    private:
        std::string _error;
        int _code;
};


}

#endif
