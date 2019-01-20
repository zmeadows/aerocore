#pragma once

#include "System.hpp"

namespace Twister {

struct Tag {};

//TODO: use enum class
enum State {
    Relocating,
    PauseBeforeFiring,
    Firing,
    PauseAfterFiring
};

void generate(void);

class StateMachineSystem final : public System {
public:
    void run(float dt) final;
    StateMachineSystem(void);
};

}
