#include <iostream>
#include <rainbow/rainbow.hpp>

using namespace rb;

namespace { bool exit_rainbow; }

void sdl_quit(int event)
{
    exit_rainbow = true;
}

int main(int argc, char** argv)
{
    exit_rainbow = false;
    Display display("Rainbow");
    display.resolution(640, 480);
    Input input;

    input.bind(SDL_QUIT, sdl_quit);
    input.bind('q', sdl_quit);

    while (!exit_rainbow) {
        input.run();
        display.run();
    }

    return 0;
}
