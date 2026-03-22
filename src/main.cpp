#include "game/game.hpp"
#include "logger.hpp"

constexpr glm::ivec2 SCREEN_SIZE = { 1280, 720 };

int main(void) {
    lg::set_file_level(lg::LogLevel::Debug);
    lg::set_stdout_level(lg::LogLevel::Info);
    lg::set_file("output.log");

    auto game = Game("Game", SCREEN_SIZE);
    lg::info("Game initialized");
    {
        game.run();
    }
    lg::info("Game terminated");
}
