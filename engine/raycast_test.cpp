#include "unity.h"

#ifndef RAYCAST_TEST_H
#define RAYCAST_TEST_H

#include "raycast.h"

bool floatEquality(float a, float b, float epsilon) {
  return fabs(a - b) < epsilon;
}

bool testIntersectRayTriangle() {
  bool result = false;
  vec3 intersection = glm::vec3(0);

  result = intersectRayTriangle(glm::vec3(0), glm::vec3(0, 0, -1),
                                glm::vec3(1, 0, -1), glm::vec3(0, 1, -1),
                                glm::vec3(-1, -1, -1), &intersection);
  if (result != true) {
    printf("test_1: result isn't true\n");
    return true;
  }

  if (!floatEquality(intersection.x, 0.f, 0.01)) {
    printf("test_1: x value mismatch: %f != %f\n", intersection.x, 0.f);
    return true;
  }

  if (!floatEquality(intersection.y, 0.f, 0.01)) {
    printf("test_1: y value mismatch: %f != %f\n", intersection.y, 0.f);
    return true;
  }

  if (!floatEquality(intersection.z, -1.f, 0.01)) {
    printf("test_1: z value mismatch: %f != %f\n", intersection.z, -1.f);
    return true;
  }

  result = intersectRayTriangle(
      glm::vec3(0), glm::normalize(glm::vec3(-13.18f, 16.89, 4)),
      glm::vec3(-7.34, 0.46, 5), glm::vec3(-3.18, 6.48, 0),
      glm::vec3(-6.76, 7.67, 3), &intersection);
  if (result != true) {
    printf("test_2: result isn't true\n");
    return true;
  }

  if (!floatEquality(intersection.x, -4.57f, 0.01)) {
    printf("test_2: x value mismatch: %f != %f\n", intersection.x, -4.57f);
    return true;
  }

  if (!floatEquality(intersection.y, 5.85f, 0.01)) {
    printf("test_2: y value mismatch: %f != %f\n", intersection.y, 5.85f);
    return true;
  }

  if (!floatEquality(intersection.z, 1.38f, 0.01)) {
    printf("test_2: z value mismatch: %f != %f\n", intersection.z, 1.38f);
    return true;
  }

  return false;
}

#endif