#include <iostream>
#include <thread>
#include <rainbow/gl.h>
#include <rainbow/rainbow.hpp>

using namespace rb;

struct Shader_entry {
    const char* name;
    const char* vert;
    const char* frag;
    Shader* shader;
};

int main(int argc, char** argv)
{
    bool quit = false;

    // Initialize the display
    Display display("Rainbow");
    display.resolution(640, 480);

    // Handle our input binding in its base form
    Input input;
    input.bind('q', [&quit](int event) { quit = true; });

    // Handle some basic shader tests
    Renderer& renderer = *display.renderer;
    Shader_entry shaders[] = {
        {"barebones", "game/shaders/barebones.vert", 
            "game/shaders/barebones.frag", nullptr},
    };

    // Load the shaders
    for (Shader_entry& e: shaders) {
        int n = renderer.add_shader(e.name, e.vert, e.frag, false);
        Shader* shader = renderer.shaders[n].get();
        if (!shader->valid) {
            std::cerr << shader->error << std::endl;
            return 3;
        }
        e.shader = shader;
    }

    // Draw a test square
    renderer.tests.color_square();

    // render a test square with the 
    Shader* barebones = shaders[0].shader;
    barebones->attrib("position").v2d(5, 0);
    barebones->attrib("color").v3d(5, 2);
    barebones->use();

    // Run the actual engine
    do {
        input.run();
        display.clear();
        display.run();
        display.end_frame();
    } while(!quit);

    return 0;
}
