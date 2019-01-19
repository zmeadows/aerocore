#pragma once

#include "System.hpp"

class LinearPathSystem final : public System {
public:
    void run(float dt) final;

    LinearPathSystem(void);
};

