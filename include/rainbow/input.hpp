#pragma once
#ifndef RAINBOW_INPUT_HPP
#define RAINBOW_INPUT_HPP

#include <map>
#include <vector>
#include <functional>
#include <rainbow/display/display.hpp>

namespace rb {

typedef std::function<void (int)> Event_function;
typedef std::vector<Event_function> Event_functions;
typedef std::map<int, Event_functions> Event_map;

struct Mouse_state {
    float speed;
    int x, y;
    float theta, phi;
    uint8_t b;
};

class Input {
public:
    Input(const Display& display);
    void flush_events();
    void grab_cursor();
    void release_cursor();
    void show_cursor();
    void hide_cursor();
    void run();
    Input& bind(int event, const Event_function& function);
public:
    Event_map event_callbacks;
    Mouse_state mouse;
    const Display& display;
    const Timing& timing;
};

}

#endif // RAINBOW_INPUT_HPP
