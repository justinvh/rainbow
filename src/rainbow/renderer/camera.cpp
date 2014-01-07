#include <iostream>
#include <rainbow/renderer/renderer.hpp>
#include <rainbow/renderer/camera.hpp>

using namespace rb;

Camera::Camera(Renderer* renderer, bool make_active)
{
    state.theta = state.phi = 0.0f;
    state.fov = 45.0f;
    state.aspect_ratio = 480.0f / 640.0f;
    state.roll = 0.0f;
    up = glm::vec3(0.0f, 0.0f, -1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    update_perspective();

    shader = renderer->get_or_create_shader("camera");
    shader->attrib("position").vec3(11, 0);
    shader->attrib("color").vec3(11, 3);
    shader->use();
    state.model = shader->uniform("model");
    state.view = shader->uniform("view");
    state.proj = shader->uniform("proj");

    // Initialize basic models
    model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    state.model.mat4(model);

    // Set our initial position and look somewhere
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    look(0.0, 0.0);

    // make active
    if (make_active)
        renderer->camera = this;
}

void Camera::move(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
}

void Camera::look(float phi, float theta)
{
    state.theta += theta;
    state.phi += phi;

    const float right_vec_angle = 3.14f / 2.0f;
    direction = glm::vec3(cos(state.phi) * sin(state.theta),
                                sin(state.phi),
                                cos(state.phi) * cos(state.theta));

    right = glm::vec3(sin(state.theta - right_vec_angle),
                      0.0f,
                      cos(state.theta - right_vec_angle));

    up = glm::cross(direction, right);
    view = glm::rotate(glm::mat4(1.0f),
                       -state.roll * 90.0f, glm::vec3(0.0f, 1.0f, 1.0f));

    glm::vec3 p = position;
    p.z += state.roll;

    view *= glm::lookAt(p, p + direction, up);

    // Set our view and projection matrices
    state.view.mat4(view);
    state.proj.mat4(projection);

    /*
    std::cout << "state.phi:   " << state.phi << " "
              << "state.theta: " << state.theta << " "
              << "x y z: " << p.x << " "
              << p.y << " " << p.z << " "
              << "roll: " << state.roll << std::endl;
    */
}

void Camera::fov(float degrees)
{
    state.fov = degrees;
}

void Camera::aspect_ratio(float ratio)
{
    state.aspect_ratio = ratio;
}

void Camera::update_perspective()
{
    projection = glm::perspective(state.fov,
        state.aspect_ratio, 0.1f, 1000.0f);
}

void Camera::move_right()
{
    position -= right * 0.1f;
}

void Camera::move_left()
{
    position += right * 0.1f;
}

void Camera::move_forward()
{
    position += direction * 0.1f;
}

void Camera::move_backward()
{
    position -= direction * 0.1f;
}

void Camera::roll(float angle)
{
    state.roll -= angle / 90.0f;
    if (state.roll > 0.5f)
        state.roll = 0.5f;
    else if (state.roll < -0.5f)
        state.roll = -0.5f;
}
