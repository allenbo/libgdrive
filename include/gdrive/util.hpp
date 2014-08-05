#ifndef __GDRIVE_UTIL_HPP__
#define __GDRIVE_UTIL_HPP__
#include <string>
#include <map>
#include <set>
#include <vector>
#include <cctype>
#include <string.h>
#include <stdlib.h>
#include "common/all.hpp"

#define UNSAFE " $&+,/:;=@\"<>#%{}|\\^~[]`"

namespace GDRIVE {

class URLHelper {
    public:
        static bool check_unsafe(char c) {
            const char* p = UNSAFE;
            while (*p != '\0') {
                if (*p++ == c) return true;
            }
            return false;
        }
        static std::string encode(std::string url) {
            return encode(url.c_str());
        }

        static std::string encode(const char* url) {
            COMMON::VarString vs;
            const char* p = url;
            while (*p != '\0') {
                if (check_unsafe(*p)) {
                    vs.append('%');
                    char tmp[4] = "";
                    sprintf(tmp, "%02X", *p);
                    vs.append(tmp);
                } else {
                    vs.append(*p);
                }
                p ++;
            }
            return vs.toString();
        }

        static std::string encode(std::map<std::string, std::string> & body) {
            COMMON::VarString vs;
            for(std::map<std::string, std::string>::iterator iter = body.begin(); iter != body.end(); iter ++) {
                vs.append(iter->first).append('=').append(encode(iter->second)).append('&');
            }
            if (body.size() > 0) {
                return vs.drop().toString();
            } else {
                return "";
            }
        }
};


}

#endif
