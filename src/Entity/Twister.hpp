#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"
#include "Component/BulletStream.hpp"

namespace Twister {

struct Tag {};
void generate(ComponentManager*, SpriteCache*);

struct StateMachineSystem final : System {
    StateMachineSystem() : System("Twister::StateMachine") {}
    void run(ComponentManager*, f32 dt);
    SUBSCRIBE(Entity, StateTransition, Twister::Tag);
};

}
