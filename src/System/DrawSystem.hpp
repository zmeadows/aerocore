#pragma once

#include "Entity.hpp"
#include "Globals.hpp"

class DrawSystem final : public System {
    std::array<double, 100> m_fpsHistory;
    Uint64 m_lastFrameTicks;
    size_t m_modFrame;
    double m_currentFpsAvg;

public:
    void run(float) final;

    DrawSystem() :
        System("Draw"),
        m_fpsHistory({{0.0}}),
        m_lastFrameTicks(SDL_GetPerformanceCounter()),
        m_modFrame(0) {
        get_manager()->subscribe<Entity,Sprite>(this);
    }
};

void draw_background(void);
