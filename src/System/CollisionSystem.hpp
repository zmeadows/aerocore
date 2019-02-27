#pragma once

#include "Engine/System.hpp"
#include "unstd/DynamicArray.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"

#include <mutex>

class QuadTree;

struct CollisionSystem final : SimpleParallelSystem {
    CollisionSystem() : SimpleParallelSystem("Collision") {}
    void pre(ComponentManager*) final;
    void par_run(ComponentManager*, const Slice<UUID>& entities, f32 dt) final;
    void post(ComponentManager*) final;

    struct Collision {
        UUID friendly;
        UUID enemy;
    };

    std::mutex m_mutex;
    DynamicArray<Collision> m_collisions;

    SUBSCRIBE(Entity, CollisionData, FriendlyTag);
};

