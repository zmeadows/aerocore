#pragma once

#include "Entity.hpp"
#include "Globals.hpp"

class CollisionSystem final : public System {
    std::vector<UUID> m_candidates;
public:
    void run(float dt) final;

    CollisionSystem(void) : System("Collision")
    {
        get_manager()->subscribe<CollisionData, FriendlyTag>(this);
        m_candidates.reserve(256);
    }
};
