#include "stdint.h"
#include "stdio.h"

#include "unity.h"

#include "main.cpp"
#include <windows.h>

#include "os/win/win.h"

// #include "test_rect_overlap.cpp"
// #include "test_sscanf.cpp"
// #include "projectile_pool_test.cpp"
#include "test_str8_to_str32.cpp"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
            int nShowCmd) {
  runTest();
  return 0;
};