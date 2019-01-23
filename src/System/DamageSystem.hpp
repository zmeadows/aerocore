#pragma once

#include "System.hpp"

#include <vector>

class DamageSystem final : public System {
public:
    void run(float dt) final;
    DamageSystem(void);
};
