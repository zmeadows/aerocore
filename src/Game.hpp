#pragma once

#include <memory>

#include "aerocore.hpp"

#include "QuadTree.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "SystemManager.hpp"

class Game {
    std::unique_ptr<SystemManager> SM;
    std::unique_ptr<InputManager> IM;

    bool paused = false;

    Uint32 m_preSystemRunTicks;
    Uint32 m_postSystemRunTicks;
    Uint32 m_preFlipTicks;
    Uint32 m_postFlipTicks;
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
