#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"

class QuadTree;

struct QuadTreeUpdateSystem final : SimpleParallelSystem {
    QuadTree* QT;
    QuadTreeUpdateSystem(QuadTree* _QT) : QT(_QT), SimpleParallelSystem("QuadTreeUpdate") {}
    void pre() final;
    void par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt);
    SUBSCRIBE(Entity, CollisionData);
};

