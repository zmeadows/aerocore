#pragma once

#include "System.hpp"

class OffscreenBehaviorSystem final : public System {
public:
    void run(float dt) final;
    OffscreenBehaviorSystem(void);
};
