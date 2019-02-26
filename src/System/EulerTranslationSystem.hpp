#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"

struct EulerTranslationSystem final : SimpleParallelSystem {
    EulerTranslationSystem() : SimpleParallelSystem("EulerTranslation") {}
    void par_run(ComponentManager* CM, const Slice<UUID>& entities, float dt);
    SUBSCRIBE(EulerTranslation, PositionUpdate);
};

