#pragma once

#include "System.hpp"
#include "unstd/DynamicArray.hpp"
#include "Component/Common.hpp"

struct DamageSystem {
    System base = System("Damage");
    DynamicArray<UUID> followed_copy;
    SUBSCRIBE(Health, DamageEvent);
};

void run(DamageSystem&);
