#pragma once

#include "System.hpp"

struct Entity;

class BulletStreamSystem final : public System {
public:
    void run(float dt) final;
    BulletStreamSystem(void);
};


