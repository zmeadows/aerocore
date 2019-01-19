#pragma once

#include "Typedef.hpp"
#include "System.hpp"

class DrawSystem final : public System {
    f32 m_fpsHistory[100];
    u64 m_lastFrameTicks;
    u64 m_modFrame;
    f32 m_currentFpsAvg;

public:
    void run(float) final;
    DrawSystem();
};

