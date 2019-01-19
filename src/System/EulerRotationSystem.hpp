#pragma once

#include "System.hpp"

class EulerRotationSystem final : public System {
public:
    void run(float dt) final;
    EulerRotationSystem(void);
};
