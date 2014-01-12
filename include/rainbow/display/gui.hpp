#pragma once
#ifndef RAINBOW_DISPLAY_GUI_HPP
#define RAINBOW_DISPLAY_GUI_HPP

#include <iostream>

namespace rb {

class Gui {
public:
    void draw(int x, int y,
              const std::string& message,
              const std::string& shader_name = "gui");

public:
    std::string source;

public:
    static const char* name() { return "Gui"; }
};

}

#endif // RAINBOW_DISPLAY_GUI_HPP
