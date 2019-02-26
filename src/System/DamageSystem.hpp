#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"

#include "unstd/DynamicArray.hpp"

struct DamageSystem final : System {
    DamageSystem() : System("Damage") {}
    void run(ComponentManager* CM, float dt);
    ArraySet<UUID> followed_copy;
    SUBSCRIBE(Health, DamageEvent);
};
