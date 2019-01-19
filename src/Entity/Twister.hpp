#pragma once

#include "Component/Common.hpp"
#include "Globals.hpp"
#include "Component/StateTransition.hpp"

#include <string>

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
    StateMachineSystem(void) : System("Twister::StateMachine") {
        get_manager()->subscribe<Entity, StateTransition, Twister::Tag>(this);
    }

    void run(float dt) final;
};

}
