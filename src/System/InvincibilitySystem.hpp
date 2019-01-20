#pragma once

#include "System.hpp"

class InvincibilitySystem final : public System {
public:
    void run(float) final;
    InvincibilitySystem();
};

