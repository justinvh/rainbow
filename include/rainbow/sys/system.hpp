#pragma once
#ifndef RAINBOW_SYS_SYSTEM_HPP
#define RAINBOW_SYS_SYSTEM_HPP

namespace rb {
namespace os {

enum class Platform {
    apple,
    windows,
    linux,
    freebsd,
    unix,
    unknown
};

inline
constexpr bool is_apple()
{
#ifdef __APPLE__
    return true;
#else
    return false;
#endif
}

inline
constexpr bool is_windows()
{
#if _WIN32 || _WIN64
    return true;
#else
    return false;
#endif
}

inline
constexpr bool is_unix()
{
#if __UNIX__
    return true;
#else
    return false;
#endif
}

inline
constexpr bool is_linux()
{
#ifdef __LINUX__
    return true;
#else
    return false;
#endif
}

inline
constexpr bool is_freebsd()
{
#ifdef __FreeBSD__
    return true;
#else
    return false;
#endif
}

inline
constexpr Platform build_os()
{
    return is_apple() ?  Platform::apple :
           is_windows() ? Platform::windows :
           is_linux() ? Platform::linux :
           is_freebsd() ? Platform::freebsd :
           is_unix() ? Platform::unix :
           Platform::unknown;
}

inline
constexpr bool is_unknown()
{
    return build_os() == Platform::unknown;
}

inline
constexpr bool is_os(Platform os)
{
    return os == Platform::apple ?  is_apple() :
           os == Platform::windows ? is_windows() :
           os == Platform::linux ? is_linux() :
           os == Platform::freebsd ? is_freebsd() :
           os == Platform::unix ? is_unix() :
           os == Platform::unknown ? is_unknown() :
           false;
}

inline
constexpr const char* os_as_string()
{
    return is_apple() ? "Apple " __VERSION__ :
           is_windows() ? "windows" :
           is_linux() ? "linux" :
           is_freebsd() ? "freebsd" :
           is_unix() ? "unix" :
           "unknown";
}

} // os
} // rb

#endif // RAINBOW_SYS_SYSTEM_HPP
