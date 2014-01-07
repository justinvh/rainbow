#include <iostream>
#include <SDL2/SDL.h>
#include <rainbow/display.hpp>
#include <rainbow/input.hpp>
#include <rainbow/utils/logger.hpp>

using namespace rb;

Input::Input(const Display& display)
    : display(display), timing(display.timing)
{
    mouse.x = 0;
    mouse.y = 0;
    mouse.theta = 0.0f;
    mouse.phi = 0.0f;
    mouse.speed = 0.00005f;
    show_cursor();
    flush_events();
}

Input& Input::bind(int event, const Event_function& callback)
{
    event_callbacks[event].push_back(callback);
    return *this;
}

void Input::grab_cursor()
{
}

void Input::release_cursor()
{
}

void Input::flush_events()
{
    SDL_Event dummy[1];
    while (SDL_PeepEvents(dummy, 1, SDL_PEEKEVENT,
                SDL_USEREVENT, 1));
}

void Input::show_cursor()
{
    SDL_ShowCursor(1);
}

void Input::hide_cursor()
{
    SDL_ShowCursor(0);
}

void Input::run()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        Event_functions event_functions = event_callbacks[e.type];
        for (const Event_function& function : event_functions) {
            function(e.type);
        }
        switch(e.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                int key = e.key.keysym.sym;
                Event_functions key_functions = event_callbacks[key];
                for (const Event_function& function : key_functions) {
                    function(e.type);
                }
                break;
            }
            case SDL_QUIT:
                break;
            default:
                break;
        }
    }

    mouse.b = SDL_GetRelativeMouseState(&mouse.x, &mouse.y);
    if (abs(mouse.x) > 0 || abs(mouse.y) > 0)  {
        if (abs(mouse.x) > 100 || abs(mouse.y) > 100) {
            mouse.x = 0;
            mouse.y = 0;
        }
        mouse.theta = mouse.speed * timing.fps * mouse.x;
        mouse.phi = mouse.speed * timing.fps * mouse.y;
    } else {
        mouse.theta = 0.0f;
        mouse.phi = 0.0f;
    }
}
