#include "System/StateTransitionCleanupSystem.hpp"

#include "Engine/ComponentManager.hpp"

void StateTransitionCleanupSystem::run(ComponentManager* CM, f32 dt) {
    for (const UUID& uuid : this->followed) {
        this->followed_copy.append(uuid);
    }

    for (const UUID uuid : this->followed_copy) {
        CM->remove<StateTransition>(uuid);
    }

    this->followed_copy.clear();
}
