#pragma once

#include "System.hpp"
#include "Component/Common.hpp"

struct DestructSystem {
    System base = System("Destruct");
    DynamicArray<UUID> followed_copy;
    SUBSCRIBE(DestructTag);
};

void run(DestructSystem&);
