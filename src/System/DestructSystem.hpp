#pragma once

#include "System.hpp"

#include <vector>

class DestructSystem final : public System {
    std::vector<UUID> m_uuid_set_copy;
public:
    void run(float dt) final;

    DestructSystem(void);
};
