#pragma once

#include "System.hpp"

#include <vector>

class DamageSystem final : public System {
    std::vector<UUID> m_uuid_set_copy;
public:
    void run(float dt) final;
    DamageSystem(void);
};
