#include "raycast.h"

int main(int argc, char *argv[]) {
  bool failed = testIntersectRayTriangle();
  if (failed) {
    printf("test intersect ray triangle failed\n");
    return 0;
  }
}