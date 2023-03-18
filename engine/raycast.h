#include "unity.h"

#ifndef RAYCAST_H
#define RAYCAST_H

#include "mesh.h"

void printVec3(vec3 v) { printf("(%6.2f %6.2f %6.2f)", v.x, v.y, v.z); }

bool ray_is_intersect_triangle(vec3 origin, vec3 dir, vec3 p1, vec3 p2, vec3 p3,
                               vec3 *out);

bool ray_intersect(vec3 point, vec3 dir, vec3 *triangles, int num_triangles,
                   int stride, vec3 *out) {
  for (int i = 0; i < num_triangles; i++) {
    vec3 p0 = triangles[i * stride + 0];
    vec3 p1 = triangles[i * stride + 3];
    vec3 p2 = triangles[i * stride + 6];
    if (ray_is_intersect_triangle(point, dir, p0, p1, p2, out)) {
      return true;
    }
  }
  return false;
}

bool ray_is_intersect_triangle(vec3 origin, vec3 dir, vec3 p1, vec3 p2, vec3 p3,
                               vec3 *out) {

  dir = glm::normalize(dir);
  vec3 plane_norm = glm::normalize(glm::cross(p2 - p1, p3 - p1));
  float plane_dist = glm::dot(plane_norm, p1 - origin);

  float denom = glm::dot(dir, plane_norm);
  if (-0.0001f < denom && denom < 0.0001f) {
    return false;
  }

  float t = (plane_dist - glm::dot(origin, plane_norm)) / denom;

  float d12 = glm::dot(p1 - p2, p1 - p2);
  float d13 = glm::dot(p1 - p3, p1 - p3);
  float d23 = glm::dot(p2 - p3, p2 - p3);

  vec3 pp1, pp2, pp3;
  if (d12 >= d23 && d12 >= d13) {
    pp1 = p1;
    pp2 = p2;
    pp3 = p3;
  } else if (d23 >= d12 && d23 >= d13) {
    pp1 = p2;
    pp2 = p3;
    pp3 = p1;
  } else {
    pp1 = p3;
    pp2 = p1;
    pp3 = p2;
  }

  vec3 planar_u_vec = glm::normalize(pp2 - pp1);
  vec3 planar_v_vec = glm::normalize(
      (pp3 - pp1) - planar_u_vec * glm::dot(pp3 - pp1, planar_u_vec));

  float w = glm::dot(planar_u_vec, pp2 - pp1);
  float g = glm::dot(planar_u_vec, pp3 - pp1);
  float h = glm::dot(planar_v_vec, pp3 - pp1);

  float k1 = h / g; // slope of the line
  float k2 = h / (w - g);

  vec3 p = origin + dir * t;

  float u = glm::dot(planar_u_vec, p - pp1);
  float v = glm::dot(planar_v_vec, p - pp1);

  if (u < 0.0f || u > w || v < 0.0f || v > h) {
    return false;
  }

  if (u <= g) {
    if (v <= k1 * u) {
      *out = pp1 + planar_u_vec + planar_u_vec;
      return true;
    }
  } else {
    if (v <= h - k2 * (u - g)) {
      *out = pp1 + planar_u_vec + planar_u_vec;
      return true;
    }
  }

  return false;
}

vec3 projection(vec3 v, vec3 to) {
  return glm::dot(v, to) / glm::dot(to, to) * to;
}

vec3 intersectLinePlane(vec3 lineOrigin, vec3 lineDir, vec3 planeOrigin,
                        vec3 planeNormal) {
  glm::vec3 planeDir = planeOrigin - lineOrigin;
  return lineOrigin + glm::dot(planeDir, planeNormal) /
                          glm::dot(lineDir, planeNormal) * lineDir;
}

bool intersectRayTriangle(vec3 rayOrigin, vec3 rayDir, vec3 a, vec3 b, vec3 c,
                          vec3 *out) {
  vec3 ab = b - a;
  vec3 ac = c - a;
  vec3 bc = c - b;

  vec3 planeNormal = glm::normalize(glm::cross(c - a, b - a));
  vec3 I = intersectLinePlane(rayOrigin, rayDir, a, planeNormal);

  vec3 bary = vec3(0.0f);

  {
    vec3 v = (b - a) - projection(b - a, c - b);
    bary.x = 1 - glm::dot(v, I - a) / glm::dot(v, b - a);
  }
  {
    vec3 v = (c - b) - projection(c - b, c - a);
    bary.y = 1 - glm::dot(v, I - b) / glm::dot(v, c - b);
  }
  {
    vec3 v = (b - c) - projection(b - c, b - a);
    bary.z = 1 - glm::dot(v, I - c) / glm::dot(v, b - c);
  }

  if (0 <= bary.x && bary.x <= 1 && 0 <= bary.y && bary.y <= 1 && 0 <= bary.z &&
      bary.z <= 1) {
    *out = I;
    return true;
  }

  return false;
}
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

bool intersectRayMesh(vec3 rayOrigin, vec3 rayDir, mesh_s *mesh,
                      /*int num_triangles, int stride,*/ vec3 *out) {

  if (mesh->indices_size == 0) {
    assert(mesh->verts_size % 3 == 0);
    printf("intersecting verts\n");
    for (int i = 0; i < mesh->verts_size; i += 3) {
      printf("rayOrigin: ");
      printVec3(rayOrigin);
      printf(", rayDir: ");
      printVec3(rayDir);
      printf(", p0: ");
      printVec3(mesh->verts[i].pos);
      printf(", p1: ");
      printVec3(mesh->verts[i + 1].pos);
      printf(", p2: ");
      printVec3(mesh->verts[i + 2].pos);
      printf("\n");

      if (intersectRayTriangle(rayOrigin, rayDir, mesh->verts[i].pos,
                               mesh->verts[i + 1].pos, mesh->verts[i + 2].pos,
                               out)) {
        printf("ray intersects mesh\n");
        return true;
      }
    }
  } else {
    assert(mesh->indices_size % 3 == 0);
    printf("intersecting indices\n");
    for (int i = 0; i < mesh->indices_size; i += 3) {

      if (intersectRayTriangle(rayOrigin, rayDir,
                               mesh->verts[mesh->indices[i]].pos,
                               mesh->verts[mesh->indices[i + 1]].pos,
                               mesh->verts[mesh->indices[i + 2]].pos, out)) {
        printf("ray intersects indexed mesh");
        return true;
      }
    }
  }

  // for (int i = 0; i < mesh->; i++) {
  //   vec3 p0 = triangles[i * stride + 0];
  //   vec3 p1 = triangles[i * stride + 3];
  //   vec3 p2 = triangles[i * stride + 6];
  //   if (intersectRayTriangle(rayOrigin, rayDir, p0, p1, p2, out)) {
  //     printf("ray intersects mesh");
  //     return true;
  //   }
  // }
  return false;
}

#endif