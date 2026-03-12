add_executable(game
    main.cpp
    logger.cpp
    game/camera.cpp
    game/input.cpp
    renderer/shader.cpp
    renderer/renderer.cpp
    renderer/window.cpp
    renderer/texture.cpp
    renderer/chunk_mesh.cpp
    renderer/font.cpp
    world/block.cpp
    world/chunk.cpp
    world/section.cpp
    world/world.cpp
)

target_link_libraries(game PRIVATE
    glad
    glfw
    glm
    noise
    stb
)

target_include_directories(game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/glad/include
)
