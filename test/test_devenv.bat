@REM @echo off

if not defined DevEnvDir (
  call "B:\Visual Studio\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)
devenv ..\build_win\test_run.exe