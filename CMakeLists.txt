set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.4.1)


include_directories(include external/Vulkan-Headers/include)
include_directories(external/SDL2/include)
include_directories(external/Vulkan-Headers/include)

file(GLOB MAINSOURCES src/*.cpp)
file(GLOB RENDERSOURCES src/Engine/Renderer/*/*.cpp)

set(SOURCE_FILES ${MAINSOURCES} ${RENDERSOURCES})

add_executable(Application ${SOURCE_FILES})

if (WIN32)
    set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/external/SDL2/lib/x64/SDL2.lib")
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
endif (WIN32)

add_subdirectory(external/volk)

target_link_libraries(Application ${SDL2_LIBRARIES} volk volk_headers)