#pragma once

#include "GraphicsContext.hpp"
#include "Sprite.hpp"
#include "aerocore.hpp"

class DrawSystem final : public System {
    GraphicsContext* const GC;

    std::array<double, 100> m_fpsHistory;
    Uint64 m_lastFrameTicks;
    size_t m_modFrame;
    double m_currentFpsAvg;

public:
    void run(float) final;

    DrawSystem(ComponentManager* const CM_, GraphicsContext* const GC_) :
        System("Draw", CM_),
        GC(GC_),
        m_fpsHistory({{0.0}}),
        m_lastFrameTicks(SDL_GetPerformanceCounter()),
        m_modFrame(0) {
        CM->subscribe<CoreData>(this);
    }
};
