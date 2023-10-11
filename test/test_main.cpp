#include "stdint.h"
#include "stdio.h"

#include "unity.h"

#include "main.cpp"
#include <windows.h>

// #include "test_rect_overlap.cpp"
#include "test_sscanf.cpp"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
            int nShowCmd) {
  runTest();
  return 0;
};