@echo off

if not defined DevEnvDir (
  call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build" x64
)
devenv ..\build_win\main_win.exe