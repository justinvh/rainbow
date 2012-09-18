#ifndef RAINBOW_DISPLAY_GUISTDLIB_HPP
#define RAINBOW_DISPLAY_GUISTDLIB_HPP

#include <v8.h>
#include <string>

namespace rb {
struct Engine_state;

namespace scripting {
v8::Handle<v8::ObjectTemplate> stdlib();
bool load(Engine_state* engine, const std::string& file);
}
}

#endif // RAINBOW_DISPLAY_GUISTDLIB_HPP
