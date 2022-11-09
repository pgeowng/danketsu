#ifndef FLYCAMERA_H
#define FLYCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "matrix.h"

struct flycamera_s
{
    //   float position[3];
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);

    //   float rotation[3];
    float pitch = 0;
    float yaw = -90;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 look_dir = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 move_dir = glm::vec3(0.0f, 0.0f, 0.0f); // in local space

    float sensitivity = 0.1f;
    float zoom = 45.0f;
    float speed = 2.5f;

    //   float speed;
    //   float sensitivity;
    float fov;
    float aspect;
    float near;
    float far;

    bool fps;
};

void flycamera_init(flycamera_s *camera, bool fps = true, float fov = 45.0f, float aspect = 16.0 / 9.0, float near = 0.1f, float far = 100.0f)
{
    camera->fps = fps;
    camera->fov = fov;
    camera->aspect = aspect;
    camera->near = near;
    camera->far = far;
}

void flycamera_process_mouse_movement(flycamera_s *camera, float xoffset, float yoffset)
{
    camera->yaw += xoffset * camera->sensitivity;
    camera->pitch += -yoffset * camera->sensitivity;
    if (camera->pitch > 89.0f)
    {
        camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f)
    {
        camera->pitch = -89.0f;
    }
}

void flycamera_process_mouse_scroll(flycamera_s *camera, float yoffset)
{
    if (camera->zoom >= 1.0f && camera->zoom <= 180.0f)
        camera->zoom -= yoffset;
    if (camera->zoom <= 1.0f)
        camera->zoom = 1.0f;
    if (camera->zoom >= 180.0f)
        camera->zoom = 180.0f;
}

void flycamera_process_movement(flycamera_s *camera, int front_axis, int right_axis)
{
    camera->move_dir = glm::vec3(right_axis, 0.0f, front_axis);
}

void flycamera_update(flycamera_s *camera, float deltaTime)
{
    glm::vec3 velocity(0.0f);

    glm::vec3 front = camera->front;
    if (camera->fps)
    {
        front.y = 0.0f;
    }

    // right vector
    velocity += glm::cross(front, camera->up) * camera->move_dir.x;

    // forward vector
    velocity += front * camera->move_dir.z;

    // prevent zero vector normalization
    if (glm::length(velocity) > 0.0f)
    {
        velocity = glm::normalize(velocity);
        camera->position += velocity * camera->speed * deltaTime;
    }
};

glm::mat4 flycamera_get_view_matrix(flycamera_s *camera)
{
    camera->front = glm::vec3(
        cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch)),
        sin(glm::radians(camera->pitch)),
        sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch)));

    return matrix_look_at(camera->position, camera->position + camera->front, camera->up);
}

glm::mat4 flycamera_get_projection_matrix(flycamera_s *camera)
{
    return glm::perspective(glm::radians(camera->fov + camera->zoom), camera->aspect, camera->near, camera->far);
};

#endif