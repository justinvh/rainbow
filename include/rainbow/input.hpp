#ifndef RAINBOW_INPUT_HPP
#define RAINBOW_INPUT_HPP

#include <map>
#include <vector>
#include <functional>

namespace rb {

typedef std::function<void (int)> Event_function;
typedef std::vector<Event_function> Event_functions;
typedef std::map<int, Event_functions> Event_map;

class Input {
public:
    Input();
    void flush_events();
    void grab_cursor();
    void release_cursor();
    void show_cursor();
    void hide_cursor();
    void run();
    Input& bind(int event, const Event_function& function);
public:
    Event_map event_callbacks;
};

}

#endif // RAINBOW_INPUT_HPP
