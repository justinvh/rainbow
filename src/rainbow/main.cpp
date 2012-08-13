#include <iostream>
#include <thread>
#include <rainbow/rainbow.hpp>

using namespace rb;

namespace { bool exit_rainbow; }

void sdl_quit(int event)
{
    exit_rainbow = true;
}

Display display("Rainbow");

void run()
{
    display.run_forever();
}

int main(int argc, char** argv)
{
    display.resolution(640, 480);
    exit_rainbow = false;
    Input input;

    input.bind(SDL_QUIT, sdl_quit);
    input.bind('q', sdl_quit);
    input.run();

    std::thread renderer_thread(run);

    while (!exit_rainbow) {
        input.run();
    }

    renderer_thread.join();

    return 0;
}
