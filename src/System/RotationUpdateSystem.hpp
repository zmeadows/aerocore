#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"

struct RotationUpdateSystem final : SimpleParallelSystem {
    RotationUpdateSystem() : SimpleParallelSystem("RotationUpdate") {}
    void par_run(ComponentManager* CM, const Slice<UUID>& entities, float dt);
    SUBSCRIBE(Entity, RotationUpdate);
};

