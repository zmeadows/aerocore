#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"

struct PositionUpdateSystem final : SimpleParallelSystem {
    PositionUpdateSystem() : SimpleParallelSystem("PositionUpdate") {}
    void par_run(ComponentManager* CM, const Slice<UUID>& entities, float dt);
    SUBSCRIBE(Entity, PositionUpdate);
};

