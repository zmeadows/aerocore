#pragma once

#include "System.hpp"
#include "Base.hpp"
#include "BoundingSurface.hpp"
#include "aerocore.hpp"

class CollisionSystem final : public System {
    bool are_colliding(const UUID& uuidA, const UUID& uuidB);

public:
    void run(float dt) final;

    CollisionSystem(ComponentManager* const CM_) : System("Collision", CM_) {
        CM->subscribe<CoreData, CollisionData>(this);
    }
};
