#pragma once
#ifndef RAINBOW_UTILS_SUFFIX_HPP
#define RAINBOW_UTILS_SUFFIX_HPP

#include <string>
#include <vector>
#include <sstream>
#include <exception>

namespace std {

std::string to_string(const char* value)
{
    return std::string(value);
}

std::string to_string(const unsigned char* value)
{
    return to_string(reinterpret_cast<const char*>(value));
}

std::string to_string(std::string value)
{
    return value;
}

template <class T>
std::string to_string(std::vector<T> xs)
{
    std::stringstream ss;

    ss << "{ ";
    for (const T& s : xs) {
        ss << "\"" << s << "\" ";
    }
    ss << "}";
    return ss.str();
}

}

namespace rb {

struct StringFormat {
    std::string fmt;
    std::vector<std::string> arguments;

    StringFormat(std::string fmt)
        : fmt(fmt) {}

    template <typename T, typename ...Tail>
    std::string operator()(const T& t, Tail&&... tail)
    {
        arguments.push_back(std::to_string(t));
        return operator()(std::forward<Tail>(tail)...);
    }

    std::string operator()()
    {
        size_t found = 0;
        const std::string grep = "{}";
        auto arg_begin = arguments.begin();
        auto arg_end = arguments.end();

        while ((found = fmt.find(grep, found)) != std::string::npos) {
            if (arg_begin == arg_end)
                throw std::range_error{"args out of range for '{}'"};
            fmt.replace(found, grep.size(), *arg_begin);
            ++arg_begin;
        }

        return fmt;
    }
};

}

inline rb::StringFormat operator"" _format (const char* fmt, size_t len)
{
    return rb::StringFormat(fmt);
}

#endif // RAINBOW_UTILS_SUFFIX_HPP
