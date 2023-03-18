#include "raycast_test.cpp"

int main(int argc, char *argv[]) {
  bool failed = testIntersectRayTriangle();
  if (failed) {
    printf("test intersect ray triangle failed\n");
    return 0;
  }

  return 0;
}