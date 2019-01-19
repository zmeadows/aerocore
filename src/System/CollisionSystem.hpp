#pragma once

#include "System.hpp"

#include <vector>

class CollisionSystem final : public System {
    std::vector<UUID> m_candidates;
public:
    void run(float dt) final;
    CollisionSystem(void);
};
