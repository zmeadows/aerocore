#pragma once

#include "System.hpp"

//TODO: move /engine to src/Engine?
class StateTransitionCleanupSystem final : public System {
public:
    void run(float dt) final;
    StateTransitionCleanupSystem();
};

