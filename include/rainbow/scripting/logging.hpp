#ifndef RAINBOW_SCRIPTING_LOGGING
#define RAINBOW_SCRIPTING_LOGGING

#include <rainbow/scripting/scriptable.hpp>

namespace rb {

class Logging : public Scriptable<Logging> {
public:
    static v8::Handle<v8::Value> info(const v8::Arguments& args);
    static v8::Handle<v8::Value> debug(const v8::Arguments& args);

public:
    static const char* name() { return "Logging"; }
    static v8::Persistent<v8::FunctionTemplate>& function_tmpl();
    static Easy_accessor* accessors();
};

}

#endif // RAINBOW_SCRIPTING_LOGGING
