#pragma once

#include <memory>

#include <SDL2/SDL.h>

class InputManager;
class SystemManager;

class Game {
    std::unique_ptr<SystemManager> SM;
    std::unique_ptr<InputManager> IM;

    bool paused = false;

    Uint64 m_preSystemRunTicks;
    Uint64 m_postSystemRunTicks;
    Uint64 m_preFlipTicks;
    Uint64 m_postFlipTicks;
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
