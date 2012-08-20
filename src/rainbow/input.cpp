#include <SDL2/SDL.h>
#include <SDL2/SDL_input.h>
#include <rainbow/input.hpp>

using namespace rb;

Input::Input()
{
    show_cursor();
    flush_events();
}

void Input::bind(int event, const Event_function& callback)
{
    event_callbacks[event].push_back(callback);
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
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_QUIT:
                break;
            default:
                break;
        }
    }
}
