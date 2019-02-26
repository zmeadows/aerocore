#pragma once

#include "Engine/System.hpp"
#include "Spline.hpp"
#include "Component/Common.hpp"

struct TranslationSplineSystem final : System {
    TranslationSplineSystem() : System("TranslationSpline") {}
    void run(ComponentManager* CM, float dt);
    DynamicArray<UUID> finished;
    SUBSCRIBE(Entity, TranslationSpline, PositionUpdate);
};

