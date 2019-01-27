#pragma once

#include "System.hpp"
#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"
#include "Component/BulletStream.hpp"

namespace Twister {

struct Tag {};
void generate(void);

struct StateMachineSystem {
    System base = System("Twister::StateMachine");
    SUBSCRIBE(Entity, StateTransition, Twister::Tag);
};

void run(StateMachineSystem& self);

}
