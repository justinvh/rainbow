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
    Input input(display);

    // Handle some basic shader tests
    Renderer& renderer = *display.renderer;
    Shader* model_shader = renderer.get_or_create_shader("model");
    model_shader->attrib("color").vec3(11, 3);

    for (int i = 1; i < argc; i++) {
        std::cout << "Loading: " << argv[i] << "\n";
        Model_unique obj = load_model(argv[i], Model_format::OBJ);
        renderer.add_static_vertices(obj->verts(),
                                     obj->vert_size(),
                                     obj->elements(),
                                     obj->element_size(),
                                     [&model_shader]() {
                                        model_shader->use();
                                     });
    }

    Camera camera1(&renderer, true);
    Camera* active = &camera1;

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
         .bind('1', [&camera1, &active](int event) { active = &camera1; });
    
    // Run the actual engine
    uint64_t frame = 0;

    do {
        input.run();
        display.clear();

        active->look(input.mouse.phi, input.mouse.theta);

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
