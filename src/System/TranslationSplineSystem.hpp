#pragma once

#include "System.hpp"
#include "Spline.hpp"
#include "Component/Common.hpp"

struct TranslationSplineSystem {
    System base = System("TranslationSpline");
    DynamicArray<UUID> finished;
    SUBSCRIBE(Entity, TranslationSpline, PositionUpdate);
};

void run(TranslationSplineSystem&, f32 dt);
