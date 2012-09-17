#ifndef RAINBOW_DISPLAY_GUISTDLIB_HPP
#define RAINBOW_DISPLAY_GUISTDLIB_HPP

#include <v8.h>
#include <string>

namespace rb {
namespace scripting {
v8::Handle<v8::ObjectTemplate> stdlib();
bool load(const std::string& file);
}
}

#endif // RAINBOW_DISPLAY_GUISTDLIB_HPP
