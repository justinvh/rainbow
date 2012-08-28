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
    input.bind('q', [&quit](int event) { quit = true; })
         .bind('g', [&display](int event) { display.grab_mouse = true;  })
         .bind('r', [&display](int event) { display.grab_mouse = false; });

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

    glm::mat4 model;
    model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 view = glm::lookAt(glm::vec3( 1.0f, 1.0f, 1.0f ),
                                 glm::vec3( 0.0f, 0.0f, 0.0f ),
                                 glm::vec3( 0.0f, 0.0f, 1.0f ));
    glm::mat4 proj = glm::perspective(45.0f, 480.0f / 640.0f, 0.1f, 1000.0f);
    glm::vec3 position = glm::vec3(1.2f, 1.2f, 1.2f);

    Uniform umodel = barebones->uniform("model");
    Uniform uview = barebones->uniform("view");
    Uniform uproj = barebones->uniform("proj");

    umodel.mat4(model);
    uview.mat4(view);
    uproj.mat4(proj);
    
    float forward = 0, sides = 0, rotation = 1;
    input.bind('w', [&forward](int event) { forward -= 0.05; })
         .bind('s', [&forward](int event) { forward += 0.05; })
         .bind('a', [&sides](int event) { sides -= 0.05; })
         .bind('d', [&sides](int event) { sides += 0.05; });

    // Run the actual engine
    uint64_t frame = 0;
    const float right_vec_angle = 3.14f / 2.0f;
    do {
        input.run();
        display.clear();
        model = glm::rotate(model, rotation, glm::vec3(0.7f, 0.5f, 0.3f));

        float phi = input.mouse.phi;
        float theta = input.mouse.theta;

        glm::vec3 direction(cos(phi) * sin(theta),
                            sin(phi),
                            cos(phi) * cos(theta));

        glm::vec3 right = glm::vec3(sin(theta - right_vec_angle),
                                    0,
                                    cos(theta - right_vec_angle));

        glm::vec3 up = glm::cross(right, direction);

        view = glm::lookAt(position, position + direction, up);

        uview.mat4(view);
        umodel.mat4(model);
        frame++;
        display.run();
        display.end_frame();
    } while(!quit);

    return 0;
}
