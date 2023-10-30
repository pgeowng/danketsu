@echo off

set CommonCompilerFlags=-MT -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -FC -Z7 -Fmdanketsu.map
set CommonLinkerFlags=-opt:ref user32.lib gdi32.lib SDL2.lib SDL2main.lib glew32.lib OpenGL32.lib GLu32.lib
set HeaderLinkFlags=-I..\external\glew-2.1.0\include -I..\external\SDL2-2.28.3-VC\include -I../external/stb -I../external/glm -I../engine -I../engine/renderer
set LibraryPaths=/LIBPATH:..\external\SDL2-2.28.3-VC\lib\x64 /LIBPATH:..\external\glew-2.1.0\lib\Release\x64

mkdir ..\build_win
pushd ..\build_win
cl.exe %CommonCompilerFlags%  "../engine/main_win.cpp" %HeaderLinkFlags% /link %LibraryPaths% %CommonLinkerFlags%
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
