#pragma once

#include "System.hpp"

class RotationUpdateSystem final : public System {
public:
    void run(float dt) final;
    RotationUpdateSystem(void);
};
