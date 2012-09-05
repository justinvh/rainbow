#ifndef RAINBOW_DISPLAY_GUI_HPP
#define RAINBOW_DISPLAY_GUI_HPP

#include <iostream>

namespace rb {

class Gui {
public:
    Gui();
    void draw(int x, int y, const std::string& message);
public:
    int id;
};

}

#endif // RAINBOW_DISPLAY_GUI_HPP
