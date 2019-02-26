#include "System/DestructSystem.hpp"
#include "Engine/ComponentManager.hpp"

void DestructSystem::run(ComponentManager* CM, f32 dt) {
    for (const UUID& uuid : this->followed)
        this->followed_copy.append(uuid);

    for (const UUID& uuid : this->followed_copy) {
        CM->destroy(uuid);
    }

    this->followed_copy.clear();
}
