set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
cmake_minimum_required(VERSION 3.4.1)

if (UNIX)
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
endif (UNIX)

include_directories(include external/Vulkan-Headers/include)
include_directories(external/SDL2/include)
include_directories(external/Vulkan-Headers/include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(external/spdlog/include)
include_directories(external/rapidjson/include/rapidjson)
include_directories(external/VulkanMemoryAllocator/src)
include_directories(external/glm)

file(GLOB MAINSOURCES src/*.cpp)
file(GLOB RENDERSOURCES src/Engine/Renderer/*/*.cpp src/Engine/Renderer/*/*/*.cpp)
file(GLOB RESOURCESOURCES src/Engine/Resources/*)

set(SOURCE_FILES ${MAINSOURCES} ${RENDERSOURCES} ${RESOURCESOURCES})

add_executable(Application ${SOURCE_FILES})

if (WIN32)
    set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/external/SDL2/lib/x64/SDL2.lib")
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
endif (WIN32)

if (UNIX)
target_link_libraries(Application ${CMAKE_DL_LIBS} Threads::Threads)
endif (UNIX)

add_subdirectory(external/volk)

target_link_libraries(Application ${SDL2_LIBRARIES} volk volk_headers)