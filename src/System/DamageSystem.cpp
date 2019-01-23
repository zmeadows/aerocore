#include "System/DamageSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"

DamageSystem::DamageSystem(void) : System("Damage") {
    get_manager()->subscribe<Health, DamageEvent>(this);
}

void DamageSystem::run(float) {
    ComponentManager* CM = get_manager();

    UUIDSet::const_iterator it = m_followed.begin();

    while (it != m_followed.end()) {
        const UUID uuid = *it;

        const auto& damage = CM->get<DamageEvent>(uuid);
        auto& health = CM->get<Health>(uuid);

        health.modify_by(-1.f * damage.amount);

        if (health.current <= 0.f)
            CM->book<DestructTag>(uuid);

        it = CM->remove_in_system_loop<DamageEvent>(it, this);
    }
}
