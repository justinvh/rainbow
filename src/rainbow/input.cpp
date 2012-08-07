#include <SDL.h>
#include <rainbow/input.hpp>

using namespace rb;

Input::Input()
{
    show_cursor();
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    flush_events();
}

void Input::bind(int event, const Event_function& callback)
{
    event_callbacks[event].push_back(callback);
}

void Input::grab_cursor()
{
    SDL_WM_GrabInput(SDL_GRAB_ON);
}

void Input::release_cursor()
{
    SDL_WM_GrabInput(SDL_GRAB_OFF);
}

void Input::flush_events()
{
    SDL_Event dummy[1];
    while (SDL_PeepEvents(dummy, 1, SDL_GETEVENT, 
                          SDL_EVENTMASK(SDL_MOUSEMOTION)));
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
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_QUIT:
            case SDL_VIDEORESIZE:
            case SDL_ACTIVEEVENT:
                break;
            default:
                break;
        }
    }
}
