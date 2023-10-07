#include "stdint.h"
#include "stdio.h"

#include "unity.h"

#include "main.cpp"
#include <windows.h>

typedef struct testCase {
  f32 a[4];
  f32 b[4];
  b8 result;
} testCase;

void runCase(f32 *a, f32 *b, b8 expected) {
  b8 actual = rectOverlaps(a, b);
  if (expected != actual) {
    LogError("test failed: a=[%f,%f,%f,%f], b=[%f,%f,%f,%f], expected=%d\n",
             a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3], expected);
  }
}

void runTest() {
  {
    f32 a[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    f32 b[4] = {100.0f, 100.0f, 0.0f, 0.0f};
    b8 expected = 0;
    runCase(a, b, expected);
  }

  {
    f32 a[4] = {0.0f, 1.0f, 3.0f, 2.0f};
    f32 b[4] = {1.0f, 0.0f, 1.0f, 4.0f};
    b8 expected = 1;
    runCase(a, b, expected);
  }

  {
    // top left inside
    f32 a[4] = {0.0f, 0.0f, 10.0f, 10.0f};
    f32 b[4] = {5.0f, 5.0f, 10.0f, 10.0f};
    b8 expected = 1;
    runCase(a, b, expected);
  }

  {
    // top right inside
    f32 a[4] = {0.0f, 0.0f, 10.0f, 10.0f};
    f32 b[4] = {-5.0f, 5.0f, 10.0f, 10.0f};
    b8 expected = 1;
    runCase(a, b, expected);
  }

  {
    // bottom left inside
    f32 a[4] = {0.0f, 0.0f, 10.0f, 10.0f};
    f32 b[4] = {5.0f, -5.0f, 10.0f, 10.0f};
    b8 expected = 1;
    runCase(a, b, expected);
  }

  {
    // bottom right inside
    f32 a[4] = {0.0f, 0.0f, 10.0f, 10.0f};
    f32 b[4] = {-5.0f, -5.0f, 10.0f, 10.0f};
    b8 expected = 1;
    runCase(a, b, expected);
  }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
            int nShowCmd) {
  runTest();
  return 0;
};