#pragma once
#ifndef RAINBOW_UTILS_LOGGER_HPP
#define RAINBOW_UTILS_LOGGER_HPP

#include <iostream>
#include <rainbow/utils/itertools.hpp>
#include <rainbow/utils/suffix.hpp>

namespace rb {

enum class LogLevel {
    critical,
    error,
    warning,
    info,
    debug,
    get
};

template <class OutputIter, class = void>
class Logger; // User did not pass an output iterator tagged object

template <class OutputIter>
class Logger<OutputIter, typename is_output_iterator<OutputIter>::type> {
public:
    OutputIter sink;
    LogLevel log_level;

public:
    Logger(LogLevel log_level, OutputIter& sink)
        : sink(sink), log_level(log_level) {}

    static LogLevel level(LogLevel log_level = LogLevel::get)
    {
        static LogLevel current_level = LogLevel::info;
        if (log_level == LogLevel::get)
            return current_level;
        current_level = log_level;
        return current_level;
    }

    void operator()(std::string msg)
    {
        if (Logger<OutputIter>::level() < log_level)
            return;

        for (char i : msg)
            *sink++ = i;
    }
};

typedef std::ostream_iterator<char> DefaultLogWriter;
typedef Logger<DefaultLogWriter> DefaultLogger;

Logger<DefaultLogWriter>& get_logger(LogLevel level)
{
    static DefaultLogWriter writer_cerr(std::cerr, "");
    static DefaultLogWriter writer_clog(std::clog, "");
    static DefaultLogWriter writer_cout(std::cout, "");

    static DefaultLogger info(LogLevel::info, writer_cout);
    static DefaultLogger debug(LogLevel::debug, writer_clog);
    static DefaultLogger warning(LogLevel::warning, writer_clog);
    static DefaultLogger error(LogLevel::error, writer_cerr);
    static DefaultLogger critical(LogLevel::error, writer_cerr);

    switch (level) {
        case LogLevel::info: return info;
        case LogLevel::debug: return debug;
        case LogLevel::warning: return warning;
        case LogLevel::error: return error;
        case LogLevel::critical: return critical;
        default: return info;
    }
}

}

#define log_critical(msg) \
rb::get_logger(rb::LogLevel::critical) \
("[CRITICAL] {} <{}:{}>\n{}\n\n"_format(__FILE__, __FUNCTION__, __LINE__, msg))

#define log_error(msg) \
rb::get_logger(rb::LogLevel::error) \
("[ERROR] {} <{}:{}>\n{}\n\n"_format(__FILE__, __FUNCTION__, __LINE__, msg))

#define log_warning(msg) \
rb::get_logger(rb::LogLevel::debug) \
("[WARNING] {} <{}:{}>\n{}\n\n"_format(__FILE__, __FUNCTION__, __LINE__, msg))

#define log_info(msg) \
rb::get_logger(rb::LogLevel::info) \
("[INFO] {} <{}:{}>\n{}\n\n"_format(__FILE__, __FUNCTION__, __LINE__, msg))

#define log_debug(msg) \
rb::get_logger(rb::LogLevel::debug) \
("[DEBUG] {} <{}:{}>\n{}\n\n"_format(__FILE__, __FUNCTION__, __LINE__, msg))

#endif // RAINBOW_UTILS_LOGGER_HPP
