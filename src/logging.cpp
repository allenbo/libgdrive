#include <logging.hpp>
#include <stdlib.h>
#include <stdio.h>

namespace GDRIVE {

static const char* level_literal[] = {
    "Debug",
    "Info",
    "Warn",
    "Error",
    "Fatal",
    NULL
};

void Logger::debug(const char* funcname, const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(funcname, filename, lineno, DEBUG, fmt, va);
    va_end(va);
}

void Logger::info(const char* funcname, const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(funcname, filename, lineno, INFO, fmt, va);
    va_end(va);
}

void Logger::warn(const char* funcname, const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(funcname, filename, lineno, WARN, fmt, va);
    va_end(va);
}

void Logger::error(const char* funcname, const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(funcname, filename, lineno, ERROR, fmt, va);
    va_end(va);
}

void Logger::fatal(const char* funcname, const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(funcname, filename, lineno, FATAL, fmt, va);
    va_end(va);
}

void Logger::_log(const char* funcname, const char* filename, int line, Level level, const char* fmt, va_list va) {
    if (level < _level) return;
    char fullfmt[512];

    if (_type == LT_FUNCTION) {
        snprintf(fullfmt, 511, "%s [%s@%s|%d] %s", level_literal[level], funcname, filename, line, fmt);
    } else {
        snprintf(fullfmt, 511, "%s [%s:%s@%s|%d] %s", level_literal[level], _cname.c_str(), funcname, filename, line, fmt);
    }
    vfprintf(stderr, fullfmt, va);

    if (level == ERROR or level == FATAL) {
        exit(-1);
    }
}

}
