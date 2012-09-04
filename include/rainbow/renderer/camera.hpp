#ifndef RAINBOW_RENDERER_CAMERA_HPP
#define RAINBOW_RENDERER_CAMERA_HPP

#include <rainbow/gl.h>

namespace rb {

class Camera {
public:
    Camera();
    void move(float x, float y, float z);
    void look(float phi, float theta);
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
    glm::mat4 view;
    glm::mat4 projection;

    float fov_gl;
    float aspect_ratio_gl;
};

}

#endif // RAINBOW_RENDERER_CAMERA_HPP
