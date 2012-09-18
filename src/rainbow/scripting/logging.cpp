#include <iostream>
#include <rainbow/display.hpp>
#include <rainbow/scripting/logging.hpp>
#include <rainbow/state.hpp>
#include <rainbow/v8/easy.hpp>

using namespace std;
using namespace rb;
using namespace v8;

namespace {
Accessors logging_accessors;
Functions logging_functions;
Persistent<FunctionTemplate> logging_function_tmpl;
}

v8::Persistent<v8::FunctionTemplate>& Logging::function_tmpl()
{
    Engine_state* state = unwrap_global_pointer<Engine_state>(0);
    cout << state->display->display_name << endl;
    return logging_function_tmpl;
}

Accessors Logging::accessors()
{
    return logging_accessors;
}

Functions Logging::functions()
{
    return logging_functions;
}
