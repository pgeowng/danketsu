pwd
CXX="g++"
CFLAGS="-fdiagnostics-color=always -g"
LDLIBS="-lSDL2 -lGLEW -lGL"

CPPFILES="../engine/main.cpp"

$CXX $CFLAGS $LDLIBS $CPPFILES -o '../build/main'
