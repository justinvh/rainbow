#include <rainbow/scripting/logging.hpp>

using namespace rb;
using namespace v8;

namespace {
Easy_accessor logging_accessors[] = {
    {nullptr, nullptr, nullptr}
};

Persistent<FunctionTemplate> logging_function_tmpl;
}

v8::Persistent<v8::FunctionTemplate>& Logging::function_tmpl()
{
    return logging_function_tmpl;
}

Easy_accessor* Logging::accessors()
{
    return logging_accessors;
}
