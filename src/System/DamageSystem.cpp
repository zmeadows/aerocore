#include "System/DamageSystem.hpp"

#include "Component/Common.hpp"
#include "Engine/ComponentManager.hpp"

void DamageSystem::run(ComponentManager* CM, f32 dt) {
    this->followed_copy.copy_from(this->followed);

    for (const UUID& uuid : this->followed) {
        auto& damage = CM->get<DamageEvent>(uuid);
        auto& health = CM->get<Health>(uuid);

        health.modify_by(-1.f * damage.amount);

        if (health.current <= 0.f)
            CM->book<DestructTag>(uuid);
    }

    for (const UUID uuid : this->followed_copy) {
        CM->remove<DamageEvent>(uuid);
    }
}
