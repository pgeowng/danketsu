@echo off
rem call "B:\Visual Studio\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set CommonCompilerFlags=-MT -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -FC -Z7 -Fmwin32_handmade.map
set CommonLinkerFlags=-opt:ref user32.lib gdi32.lib

mkdir ..\build
pushd ..\build
cl.exe %CommonCompilerFlags% "../code/win32_handmade.cpp" /link %CommonLinkerFlags%
popd


rem /link -subsystem:windows,5.1 - compining for x86
rem -Gm-  - minimal rebuild
rem -MT   - no redistibutables
rem -EHa- - turn off exception handling
rem -GR-  - remove runtime type information
rem -Zi	 - source map for debugger
rem -Z7	 - old format debugger info
rem -Oi  - do intrinsic stuff
rem 4189 - not used local variables
rem 4100 - not used function arguments
rem 4281 - unnamed struct inside struct
