#pragma once

#include "Base.hpp"
#include "Entity.hpp"
#include "QuadTree.hpp"
#include "Globals.hpp"

class EulerTranslationSystem final : public System {
public:
    void run(float dt) final;

    EulerTranslationSystem(void) : System("EulerTranslation")
    {
        get_manager()->subscribe<Entity,EulerTranslation,PositionUpdate>(this);
    }
};
