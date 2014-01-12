#pragma once
#ifndef RAINBOW_STATE_HPP
#define RAINBOW_STATE_HPP

namespace rb {
class Renderer;
class Input;
class Display;

struct Engine_state {
public:
    Renderer* renderer;
    Input* input;
    Display* display;
};

}

#endif // RAINBOW_STATE_HPP
