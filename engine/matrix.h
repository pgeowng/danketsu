#ifndef MATRIX_H
#define MATRIX_H

#include "debug.h"
#include "unity.h"

glm::mat4 matrix_look_at(glm::vec3 position, glm::vec3 target,
                         glm::vec3 world_up) {
  // explanation: https://learnopengl.com/Getting-started/Camera

  // in opengl, the camera is at the origin, looking down the negative z axis
  // because of that we subtract the target from the position, not the other way
  // around
  glm::vec3 dir = glm::normalize(position - target);

  // we need to find the right and up vectors for the camera
  glm::vec3 hor = glm::normalize(glm::cross(world_up, dir));
  glm::vec3 ver = glm::normalize(glm::cross(dir, hor));

  glm::mat4 result(1.0f);

  // we subtracting position from the camera matrix, moving the camera to the
  // origin
  result = glm::translate(result, -position);

  // and apply basis vectors of camera to the matrix
  result = glm::mat4(glm::transpose(glm::mat3(hor, ver, dir))) * result;

  // in result we have the transformation, that moves the world to the camera
  // origin and rotates it to the direction of the camera
  return result;
}

m4 m4LookAt(m4 out, v3 position, v3 target, v3 worldUp) {
  // explanation: https://learnopengl.com/Getting-started/Camera

  // in opengl, the camera is at the origin, looking down the negative z axis
  // because of that we subtract the target from the position, not the other way
  // around
  f32 dir[3];
  v3Copy(dir, position);
  v3Subtract(dir, target);
  v3Normalize(dir);

  // we need to find the right and up vectors for the camera
  f32 hor[3];
  v3Copy(hor, worldUp);
  v3Cross(hor, dir);
  v3Normalize(hor);

  f32 ver[3];
  v3Copy(ver, dir);
  v3Cross(ver, hor);
  v3Normalize(ver);


  // we subtracting position from the camera matrix, moving the camera to the
  // origin

  // f32 result[16];
  m4One(out);

  f32 p[3];
  v3Copy(p, position);
  v3Neg(p);

  m4Translate(out, p);


  // and apply basis vectors of camera to the matrix
  // result = glm::mat4(glm::transpose(glm::mat3(hor, ver, dir))) * result;

  // in result we have the transformation, that moves the world to the camera
  // origin and rotates it to the direction of the camera
  return out;
}

// just weird camera with unchanged local y axis
glm::mat4 _matrix_look_at_broken_y(glm::vec3 position, glm::vec3 target,
                                   glm::vec3 local_up) {
  glm::vec3 dir = glm::normalize(position - target);
  glm::vec3 hor = glm::normalize(glm::cross(local_up, dir));

  glm::mat4 result(1.0f);
  result = glm::translate(result, -position);
  result = glm::mat4(glm::transpose(glm::mat3(hor, local_up, dir))) * result;

  return result;
}

#endif