#pragma once

#include "Entity.hpp"
#include "Globals.hpp"

class DestructSystem final : public System {
    std::vector<UUID> m_uuid_set_copy;
public:
    void run(float dt) final;

    DestructSystem(void) : System("Destruct")
    {
        get_manager()->subscribe<DestructTag>(this);
    }
};
