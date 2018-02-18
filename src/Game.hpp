#pragma once

#include <memory>

#include "aerocore.hpp"

#include "QuadTree.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "SystemManager.hpp"

class Game {
    // TODO: create custom ComponentManager?
    std::unique_ptr<ComponentManager> CM;
    std::unique_ptr<GraphicsContext> GC;
    std::unique_ptr<SystemManager> SM;
    std::unique_ptr<QuadTree> m_quadTree;
    std::unique_ptr<InputManager> IM;

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
