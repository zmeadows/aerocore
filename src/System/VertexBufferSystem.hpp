#pragma once

#include "System.hpp"

class VertexBufferSystem final : public System {
public:
    void run(float dt) final;
    VertexBufferSystem(void);
};
