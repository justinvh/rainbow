#ifndef RAINBOW_RENDERER_CAMERA_HPP
#define RAINBOW_RENDERER_CAMERA_HPP

#include <rainbow/gl.h>

namespace rb {

class Camera {
public:
    Camera();
    void move(float x, float y, float z);
    void look(float phi, float theta);
    void roll(float angle);
    void fov(float degrees);
    void aspect_ratio(float ratio);
    void update_perspective();
    void move_right();
    void move_left();
    void move_forward();
    void move_backward();
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;
    glm::mat4 view;
    glm::mat4 projection;

    struct State {
        float fov;
        float aspect_ratio;
        float theta;
        float phi;
        float roll;
    } state;
};

}

#endif // RAINBOW_RENDERER_CAMERA_HPP
