#ifndef __GDRIVE_UTIL_HPP__
#define __GDRIVE_UTIL_HPP__

#include <string.h>
#include <stdlib.h>

#define BUFSIZE 1024
#define MALLOC(type, ptr, size) do {\
    if (((ptr) = (type*)malloc(size)) == NULL) {\
        fprintf(stderr, "Run out of memory at %s, %d\n", __FILE__, __LINE__);\
        exit(-1);\
    }\
} while(0)

#define REALLOC(type, ptr, size)  do {\
    if (((ptr) = (type*)realloc(ptr, size)) == NULL) {\
        fprintf(stderr, "Run out of memory at %s, %d\n", __FILE__, __LINE__);\
        exit(-1);\
    }\
} while(0)


#define UNSAFE " $&+,/:;=@\"<>#%{}|\\^~[]`"

namespace GDRIVE {

class VarString {
    public:
        VarString() {
            MALLOC(char, _p, BUFSIZE);
            _cur = _p;
            _capacity = BUFSIZE;
        }
        
        inline VarString& append(char c) {
            if (_cur - _p == _capacity - 1) {
                REALLOC(char, _p, _capacity * 2);
                _cur = _p + _capacity - 1;
                _capacity *= 2;
            }

            *_cur++ = c;
            return *this;
        }

        VarString& append(const char* p) {
            int len = strlen(p);
            return append(p, len);
        }

        VarString& append(const std::string str) {
            return append(str.c_str());
        }

        VarString& append(const char* p, const char c) {
            append(p);
            append(c);
            return *this;
        }

        VarString& append(const std::string str, const char c) {
            return append(str.c_str(), c);
        }

        VarString& append(const char* p1, const char c, const char* p2) {
            append(p1);
            append(c);
            append(p2);
            return *this;
        }

        VarString& append(const std::string str1, const char c, const std::string str2) {
            return append(str1.c_str(), c, str2.c_str());
        }

        VarString& append(const char* p, int n) {
            if (_cur - _p + n > _capacity - 1) {
                int cur_size = _cur - _p;
                int new_length = 0;
                if (_cur - _p + n < _capacity * 2 - 1) {
                    new_length = _capacity * 2; 
                } else {
                    new_length = cur_size + n + BUFSIZE;
                }

                REALLOC(char, _p, new_length);
                _cur = _p + cur_size;
                _capacity = new_length;
            }
            memcpy(_cur, p, n); 
            _cur += n;
            return *this;
        }

        inline std::string toString() {
            std::string s(_p, _cur - _p);
            return s;
        }

        inline int size() const { return _cur - _p; }

        ~VarString() {
            free(_p);
        }
    private:
        char* _p;
        char* _cur;
        int _capacity;
        VarString(const VarString&);
        VarString operator=(const VarString&);
};

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
            VarString vs;
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
};

}

#endif
