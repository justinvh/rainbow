#include <iostream>
#include <thread>

#include <rainbow/rainbow.hpp>

using namespace rb;

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << argv[0] << " font message" << std::endl;
        return 2;
    }

    bool quit = false;

    // Initialize the display
    Display display("Rainbow");
    display.resolution(640, 480);
    
    // Handle our input binding in its base form
    Input input(display);
    input.bind('q', [&quit](int event) { quit = true; })
         .bind('g', [&display](int event) { display.grab_mouse = true;  })
         .bind('r', [&display](int event) { display.grab_mouse = false; });

    Engine_state state;
    state.display = &display;
    state.renderer = display.renderer;
    state.input = &input;

    scripting::load(&state, "game/js/gui.js");

    // Run the actual engine
    do {
        input.run();
        display.clear();
        display.run();
        display.end_frame();
    } while(!quit);

    return 0;
}
