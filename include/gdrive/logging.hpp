#ifndef __JCONER_LOGGING_HPP__
#define __JCONER_LOGGING_HPP__

#include <stdarg.h>
#include <string>

namespace GDRIVE {
#define CLASS_MAKE_LOGGER \
    private:\
            Logger _logger;
#define CLASS_INIT_LOGGER(classname, level)\
    _logger.setClassName(classname);\
    _logger.setLevel(level);
#define CLOG_DEBUG(...) _logger.debug( __func__, __FILE__, __LINE__, __VA_ARGS__)
#define CLOG_INFO(...) _logger.info( __func__, __FILE__, __LINE__, __VA_ARGS__)
#define CLOG_WARN(...) _logger.warn(  __func__,__FILE__, __LINE__, __VA_ARGS__)
#define CLOG_ERROR(...) _logger.error(  __func__,__FILE__, __LINE__, __VA_ARGS__)
#define CLOG_FATAL(...) _logger.fatal(  __func__,__FILE__, __LINE__, __VA_ARGS__)


#define FUNC_MAKE_LOGGER GDRIVE::Logger __func_logger__;
#define FUNC_LOGGER_SET_LEVEL(level) __func_logger__.setLevel(level);
#define FLOG_DEBUG(...) __func_logger__.debug( __func__, __FILE__, __LINE__, __VA_ARGS__)
#define FLOG_INFO(...) __func_logger__.info( __func__, __FILE__, __LINE__, __VA_ARGS__)
#define FLOG_WARN(...) __func_logger__.warn(  __func__,__FILE__, __LINE__, __VA_ARGS__)
#define FLOG_ERROR(...) __func_logger__.error(  __func__,__FILE__, __LINE__, __VA_ARGS__)
#define FLOG_FATAL(...) __func_logger__.fatal(  __func__,__FILE__, __LINE__, __VA_ARGS__)


enum Level {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    FATAL
};

enum LoggerType {
    LT_CLASS,
    LT_FUNCTION
};

class Logger {
    public:
        Logger()
            : _level(INFO), _type(LT_FUNCTION) {}

        Logger(Level level)
            : _level(level), _type(LT_FUNCTION) {}

        void setLevel(Level level) { _level = level; }
        void setClassName(std::string cname) { _cname = cname; _type = LT_CLASS;}

        void debug(const char* funcname, const char* filename, int lineno, const char* fmt, ...);
        void info(const char* funcname, const char* filename, int lineno, const char* fmt, ...);
        void warn(const char* funcname, const char* filename, int lineno, const char* fmt, ...);
        void error(const char* funcname, const char* filename, int lineno, const char* fmt, ...);
        void fatal(const char* funcname, const char* filename, int lineno, const char* fmt, ...);

    private:
        Level _level;
        std::string _cname;
        LoggerType _type;
        void _log(const char* funcname, const char* filename, int lineno, Level level, const char* fmt, va_list va);
};

}

#endif
