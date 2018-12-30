#pragma once

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include "System.hpp"

class InputManager;
class SystemManager;

class Game {
    std::vector<std::unique_ptr<System>> systems;
    std::unique_ptr<InputManager> IM;

    bool paused = false;

    Uint64 m_preSystemRunTicks;
    Uint64 m_postSystemRunTicks;
    size_t m_frames_elapsed = 0;

    bool processInput(void);
    bool tick(void);

    Uint32 last_asteroid_time = 0;

public:
    Game(void);

    void run(void) {
        while (!tick())
            continue;
    }

    ~Game(void);
};
