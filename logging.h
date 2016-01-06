#ifndef LOGGING_H
#define LOGGING_H

#include "systemmanager.h"
#include <string>
#include <fstream>

enum LogLevel
{
    LOG_NONE,
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG
};

class Logging : public SystemBase
{
public:
    Logging(SystemManager *mgr);
    ~Logging();

    void Log(LogLevel level, std::string msg, ...);
    void SetLogLevel(LogLevel level);
    LogLevel GetLogLevel();

private:
    std::fstream logfile_;
    LogLevel loglevel_;
};

// Provide C-style string formatting for std::string
std::string string_format(const std::string fmt_str, ...);
#endif
