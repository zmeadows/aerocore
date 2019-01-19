#include "System/DestructSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"

DestructSystem::DestructSystem(void) : System("Destruct")
{
    get_manager()->subscribe<DestructTag>(this);
}

void DestructSystem::run(float) {
    ComponentManager* CM = get_manager();

    for (const UUID uuid : m_followed)
        m_uuid_set_copy.push_back(uuid);

    for (const UUID& uuid : m_uuid_set_copy) {
        CM->destroy(uuid);
    }

    m_uuid_set_copy.clear();
}
