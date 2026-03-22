add_executable(game
    game/camera.cpp
    game/input.cpp
    game/game.cpp
    renderer/shader.cpp
    renderer/window.cpp
    renderer/texture.cpp
    renderer/chunk_mesh.cpp
    renderer/chunk_renderer.cpp
    renderer/font.cpp
    world/block.cpp
    world/chunk.cpp
    world/section.cpp
    world/world.cpp
    main.cpp
    logger.cpp
    player.cpp
)

target_include_directories(game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/glad/include
)

target_compile_options(game PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wno-unused-parameter
    -O2
)

target_link_libraries(game PRIVATE
    glad
    glfw
    glm
    noise
    stb
)
