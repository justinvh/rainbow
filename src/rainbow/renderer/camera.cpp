#include <iostream>
#include <rainbow/renderer/camera.hpp>

using namespace rb;

Camera::Camera()
{
    theta_gl = phi_gl = 0.0f;
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
    theta_gl += theta;
    phi_gl += phi;

    const float right_vec_angle = 3.14f / 2.0f;
    direction = glm::vec3(cos(phi_gl) * sin(theta_gl),
                          sin(phi_gl),
                          cos(phi_gl) * cos(theta_gl));

    glm::vec3 right(sin(theta_gl - right_vec_angle),
                    0.0f,
                    cos(theta_gl - right_vec_angle));

    up = glm::cross(direction, right);
    view = glm::lookAt(position, position + direction, up);

    std::cout << "phi_gl:   " << phi_gl << " "
              << "theta_gl: " << theta_gl << " "
              << "x y z: " << position.x << " " 
              << position.y << " " << position.z << std::endl;
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
