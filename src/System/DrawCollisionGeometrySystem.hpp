#pragma once

#include "System.hpp"

class DrawCollisionGeometrySystem final : public System {
public:
    void run(float) final;
    DrawCollisionGeometrySystem();
};
