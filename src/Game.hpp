#pragma once

#include <memory>
#include <vector>

#include "InputManager.hpp"
#include "Entity/Player.hpp"

class System;

class Game {
    std::vector<std::unique_ptr<System>> systems;
    PlayerInput input;
    PlayerState state;

    u64 m_preSystemRunTicks;
    u64 m_postSystemRunTicks;
    u64 m_frames_elapsed;
    f32 time_elapsed;

    bool poll_input(void);
    bool tick(void);

    u64 last_asteroid_time;

public:
    Game(void);

    void run(void) {
        while (!tick())
            continue;
    }

    ~Game(void);
};
