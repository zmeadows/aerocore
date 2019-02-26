#pragma once

#include "Engine/System.hpp"
#include "unstd/DynamicArray.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"

#include <mutex>

class QuadTree;

struct CollisionSystem final : SimpleParallelSystem {
    DynamicArray<UUID> collision_candidates;
    CollisionSystem() : SimpleParallelSystem("Collision") {}
    void par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt);
    SUBSCRIBE(Entity, CollisionData, FriendlyTag);
    std::mutex m_mutex;
};

