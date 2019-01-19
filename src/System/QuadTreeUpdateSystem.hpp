#pragma once

#include "System.hpp"

class QuadTreeUpdateSystem final : public System {
public:
    void run(float dt) final;
    QuadTreeUpdateSystem(void);
};
