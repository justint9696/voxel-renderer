cmake_minimum_required(VERSION 3.31)
project(voxel_renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY  https://github.com/nothings/stb.git
    GIT_COMMIT      2fb8c5a3deb2110c89669f8d6f36e5833b556b44
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY  https://github.com/glfw/glfw.git
    GIT_TAG         3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY  https://github.com/g-truc/glm.git
    GIT_TAG         1.0.3
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    noise
    GIT_REPOSITORY  https://github.com/Reputeless/PerlinNoise.git
    GIT_TAG         v3.0.0
)
FetchContent_MakeAvailable(noise)

add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

add_library(noise INTERFACE)
target_include_directories(noise INTERFACE ${noise_SOURCE_DIR})

add_subdirectory(src)

