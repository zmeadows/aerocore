#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"

struct EulerRotationSystem final : SimpleParallelSystem {
    EulerRotationSystem() : SimpleParallelSystem("EulerRotation") {}
    void par_run(ComponentManager* CM, const Slice<UUID>& entities, float dt);
    SUBSCRIBE(Entity, EulerRotation, RotationUpdate);
};

