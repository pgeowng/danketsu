#include <windows.h>

#include "os/win/win.h"

#include "main.cpp"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
            int nShowCmd) {
  run();
  return 0;
};