@echo off

call "B:\Visual Studio\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
devenv ..\build_win\main_win.exe