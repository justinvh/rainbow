#pragma once
#ifndef RAINBOW_RENDERER_CONSOLE_HPP
#define RAINBOW_RENDERER_CONSOLE_HPP

#include <string>

namespace rb {

class Console {
public:
    bool is_opened;

public:
    Console();
    void toggle();
    void open();
    void close();
    bool run();
    bool send(std::string expression);
};

}

#endif // RAINBOW_RENDERER_CONSOLE_HPP
