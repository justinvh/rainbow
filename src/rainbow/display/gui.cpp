#include <iostream>
#include <cassert>
#include <sstream>
#include <memory>
#include <string>
#include <vector>

#include <rainbow/display/gui.hpp>
#include <rainbow/file.hpp>
#include <rainbow/v8.hpp>

using namespace std;
using namespace rb;
using namespace v8;

Handle<Value> draw(const Arguments& args);

namespace {
Accessors gui_accessors;
Functions gui_functions = {
    {"draw", &draw}
};

Persistent<FunctionTemplate> gui_function_tmpl;
}

v8::Persistent<v8::FunctionTemplate>& Gui::function_tmpl()
{
    return gui_function_tmpl;
}

Accessors Gui::accessors()
{
    return gui_accessors;
}

Functions Gui::functions()
{
    return gui_functions;
}

Handle<Value> draw(const Arguments& args)
{
    int x, y;
    string message;
    Gui* gui = unwrap<Gui>(args.Holder());
    auto obj_x = args[0],
         obj_y = args[1],
         obj_message = args[2];
    convert(obj_x, &x);
    convert(obj_y, &y);
    convert(obj_message, &message);
    gui->draw(x, y, message);
    return True();
}

void Gui::draw(int x, int y, const std::string& message)
{
    std::cout << "Drawing " << x << " " << y << ": " << message << "\n";
}

