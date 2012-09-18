#ifndef RAINBOW_DISPLAY_GUI_HPP
#define RAINBOW_DISPLAY_GUI_HPP

#include <iostream>
#include <rainbow/scripting/scriptable.hpp>

namespace rb {

class Gui : public Scriptable<Gui> {
public:
    void draw(int x, int y, const std::string& message, const std::string& shader_name = "gui");

public:
    std::string source;

public:
    static const char* name() { return "Gui"; }
    static v8::Persistent<v8::FunctionTemplate>& function_tmpl();
    static Accessors accessors();
    static Functions functions();
};
    
}

#endif // RAINBOW_DISPLAY_GUI_HPP
