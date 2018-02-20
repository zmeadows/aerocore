#pragma once

#include "aerocore.hpp"
#include "Base.hpp"
#include "Generator/Enemy.hpp"
#include "Globals.hpp"

class StabberSystem final : public System {
public:
    void run(float dt) final;

    StabberSystem() : System("Stabber")
    {
        get_manager()->subscribe<StabberData>(this);
    }
};

