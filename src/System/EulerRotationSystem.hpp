#pragma once

#include "Base.hpp"
#include "Entity.hpp"
#include "QuadTree.hpp"
#include "Globals.hpp"

class EulerRotationSystem final : public System {
public:
    void run(float dt) final;

    EulerRotationSystem(void) : System("EulerRotation")
    {
        get_manager()->subscribe<Entity,EulerRotation,RotationUpdate>(this);
    }
};
