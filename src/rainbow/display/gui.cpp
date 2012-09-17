#include <iostream>
#include <cassert>
#include <sstream>
#include <memory>

#include <rainbow/display/gui.hpp>
#include <rainbow/file.hpp>
#include <rainbow/v8.hpp>

using namespace rb;
using namespace v8;

namespace {
Easy_accessor gui_accessors[] = {
    {nullptr, nullptr, nullptr}
};

Persistent<FunctionTemplate> gui_function_tmpl;
}

v8::Persistent<v8::FunctionTemplate>& Gui::function_tmpl()
{
    return gui_function_tmpl;
}

Easy_accessor* Gui::accessors()
{
    return gui_accessors;
}

void Gui::draw(int x, int y, const std::string& message)
{
    std::cout << "Drawing " << x << " " << y << ": " << message << "\n";
}

