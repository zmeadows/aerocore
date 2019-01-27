#pragma once

#include "System.hpp"
#include "Component/Common.hpp"

struct EulerTranslationSystem {
    System base = System("EulerTranslation");
    SUBSCRIBE(EulerTranslation, PositionUpdate);
};

void run(EulerTranslationSystem&, f32 dt);
