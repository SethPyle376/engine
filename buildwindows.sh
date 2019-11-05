cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019"
cmake --build build
cp external/SDL2/lib/x64/SDL2.dll build/Debug/SDL2.dll