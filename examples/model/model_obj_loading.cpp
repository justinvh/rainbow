#include <iostream>
#include <thread>

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
    if (argc < 2) {
        std::cerr << argv[0] << " <model, ...>" << std::endl;
        return 2;
    }

    bool quit = false;

    // Initialize the display
    Display display("Rainbow");
    display.resolution(640, 480);

    // Handle our input binding in its base form
    Input input(display);

    scripting::load("game/js/gui.js");

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

    for (int i = 1; i < argc; i++) {
        std::cout << "Loading: " << argv[1] << "\n";
        Model_unique obj = load_model(argv[1], Model_format::OBJ);
        renderer.add_static_vertices(obj->verts(),
                                     obj->vert_size(),
                                     obj->elements(),
                                     obj->element_size());
    }

    // render a test square with the 
    Shader* barebones = shaders[0].shader;
    barebones->attrib("position").vec3(11, 0);
    barebones->attrib("color").vec3(11, 3);
    barebones->use();

    Camera camera1, camera2;
    Camera* active = &camera1;

    camera2.position = glm::vec3(-1.8f, 0.0f, 0.0f);
    camera2.look(0.114925f, 1.59037f);

    Uniform umodel = barebones->uniform("model");
    Uniform uview = barebones->uniform("view");
    Uniform uproj = barebones->uniform("proj");

    glm::mat4 model;
    model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    umodel.mat4(model);

    uview.mat4(active->view);
    uproj.mat4(active->projection);

    float roll = 0.0f;
    bool down = false;

    input.bind('p', [&quit](int event) { quit = true; })
         .bind('g', [&display](int event) { display.grab_mouse = true;  })
         .bind('r', [&display](int event) { display.grab_mouse = false; })
         .bind('w', [&active](int event) { active->move_forward(); })
         .bind('s', [&active](int event) { active->move_backward(); })
         .bind('a', [&active](int event) { active->move_left(); })
         .bind('d', [&active](int event) { active->move_right(); })
         .bind('e', [&active, &roll, &down](int event) { 
                    if (event == SDL_KEYUP) {
                        down = false;
                    } else {
                        roll = 0.5f; 
                        active->roll(roll); 
                        down = true;
                    }
          })
         .bind('q', [&active, &roll, &down](int event) { 
                    if (event == SDL_KEYUP) {
                        down = false;
                    } else {
                        roll = -0.5f; 
                        active->roll(roll); 
                        down = true;
                    }
          })
         .bind('1', [&camera1, &active](int event) { active = &camera1; })
         .bind('2', [&camera2, &active](int event) { active = &camera2; });
    
    // Run the actual engine
    uint64_t frame = 0;

    do {
        input.run();
        display.clear();

        active->look(input.mouse.phi, input.mouse.theta);
        uview.mat4(active->view);

        frame++;
        display.run();
        display.end_frame();

        if (!down && active->state.roll < (0.0f - 0.001f))
            active->roll(-0.5f);
        else if (!down && active->state.roll > (0.0f + 0.001f))
            active->roll(0.5f);

    } while(!quit);

    return 0;
}
