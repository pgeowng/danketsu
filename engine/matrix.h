#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 matrix_look_at(glm::vec3 position, glm::vec3 target, glm::vec3 world_up)
{
    // in opengl, the camera is at the origin, looking down the negative z axis
    glm::vec3 dir = glm::normalize(position - target);
    glm::vec3 hor = glm::normalize(glm::cross(world_up, dir));
    glm::vec3 ver = glm::normalize(glm::cross(dir, hor));

    glm::mat4 result(1.0f);
    result = glm::translate(result, -position);
    result = glm::mat4(glm::transpose(glm::mat3(hor, ver, dir))) * result;

    return result;
}

// just weird camera with unchanged local y axis
glm::mat4 _matrix_look_at_broken_y(glm::vec3 position, glm::vec3 target, glm::vec3 local_up)
{
    // in opengl, the camera is at the origin, looking down the negative z axis
    glm::vec3 dir = glm::normalize(position - target);
    glm::vec3 hor = glm::normalize(glm::cross(local_up, dir));

    glm::mat4 result(1.0f);
    result = glm::translate(result, -position);
    result = glm::mat4(glm::transpose(glm::mat3(hor, local_up, dir))) * result;

    return result;
}

#endif