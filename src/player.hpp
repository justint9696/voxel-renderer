#pragma once

#include "game/camera.hpp"
#include "game/input.hpp"

class Player {
public:
    Player() = default;
    
    void tick(const Input& input, Camera& camera, float dt);
};
