#include <rainbow/renderer/camera.hpp>

using namespace rb;

Camera::Camera()
{
    fov_gl = 45.0f;
    aspect_ratio_gl = 480.0f / 640.0f;
    up = glm::vec3(0.0f, 0.0f, 1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    update_perspective();
}

void Camera::move(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
    glm::lookAt(position, position + direction, up);
}

void Camera::look(float phi, float theta)
{
    const float right_vec_angle = 3.14f / 2.0f;
    direction = glm::vec3(cos(phi) * sin(theta),
                          sin(phi),
                          cos(phi) * cos(theta));

    glm::vec3 right(sin(theta - right_vec_angle),
                    0.0f,
                    cos(theta - right_vec_angle));

    up = glm::cross(direction, right);
    view = glm::lookAt(position, position + direction, up);
}

void Camera::fov(float degrees)
{
    fov_gl = degrees;
}

void Camera::aspect_ratio(float ratio)
{
    aspect_ratio_gl = ratio;
}

void Camera::update_perspective()
{
    projection = glm::perspective(fov_gl, aspect_ratio_gl, 0.1f, 1000.0f);
}

void Camera::move_right()
{
    position.z -= 0.1f;
}

void Camera::move_left()
{
    position.z += 0.1f;
}

void Camera::move_forward()
{
    position.x += 0.1f;
}

void Camera::move_backward()
{
    position.x -= 0.1f;
}
