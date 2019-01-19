#pragma once

#include "System.hpp"

class PositionUpdateSystem final : public System {
public:
    void run(float dt) final;
    PositionUpdateSystem(void);
};
