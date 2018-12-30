#pragma once

#include "Globals.hpp"

class SoundSystem final : public System {
public:
    void run(float dt) final;

    SoundSystem(void) : System("Sound")
    {
        get_manager()->subscribe<SoundEvent>(this);
    }
};
