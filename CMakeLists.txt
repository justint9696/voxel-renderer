cmake_minimum_required(VERSION 3.21...3.31)
project(voxel_renderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(game
    src/main.cpp
    src/logger.cpp
    src/game/camera.cpp
    src/game/input.cpp
    src/renderer/shader.cpp
    src/renderer/renderer.cpp
    src/renderer/window.cpp
    src/renderer/texture.cpp
    src/renderer/chunk_mesh.cpp
    src/world/chunk.cpp
    src/world/section.cpp
)

add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PRIVATE lib/glad/include)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

target_link_libraries(game
    glad
    glfw
    glm
)

target_include_directories(game PRIVATE
    lib/glad/include
    lib/glfw/include
    lib/glm
    lib/stb
    src
)
