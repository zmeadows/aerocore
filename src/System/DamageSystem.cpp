#include "System/DamageSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"

DamageSystem::DamageSystem(void) : System("Damage")
{
    get_manager()->subscribe<Health, DamageEvent>(this);
}

void DamageSystem::run(float) {
    ComponentManager* CM = get_manager();

	m_uuid_set_copy.clear();

    for (const UUID uuid : m_followed)
        m_uuid_set_copy.push_back(uuid);

    for (const UUID& uuid : m_followed) {
        auto& damage = CM->get<DamageEvent>(uuid);
        auto& health = CM->get<Health>(uuid);

        health.modify_by(-1.f * damage.amount);

        if (health.current <= 0.f)
            CM->book<DestructTag>(uuid);
    }

    for (const UUID uuid : m_uuid_set_copy)
        CM->remove<DamageEvent>(uuid);

}
