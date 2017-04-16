#include "core/logging.h"

#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <cstring>

 Logging::Logging(SystemManager *mgr) : SystemBase(mgr), loglevel_(LOG_INFO)
 {
     logfile_.open("log.txt", std::ios::out);
 }

Logging::~Logging()
{
    logfile_.close();
}

void Logging::Log(LogLevel level, std::string fmt_str, ...)
{
    if(level > loglevel_) return;

    switch(level)
    {
        case LOG_NONE: return; break;
        case LOG_ERROR: logfile_ << "ERROR: "; break;
        case LOG_INFO: logfile_ << "INFO: "; break;
        case LOG_DEBUG: logfile_ << "DEBUG: "; break;
        default: break;
    };

    int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    logfile_ << std::string(formatted.get()) << std::endl;
}

void Logging::SetLogLevel(LogLevel level)
{
    loglevel_=level;
}

LogLevel Logging::GetLogLevel()
{
    return loglevel_;
}


// Provide C-style string formatting for std::string
std::string string_format(const std::string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}
