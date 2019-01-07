#include "System/DestructSystem.hpp"

#include "Entity.hpp"

void DestructSystem::run(float) {
    ComponentManager* CM = get_manager();

    m_uuid_set_copy.clear();
    for (const UUID uuid : m_followed)
        m_uuid_set_copy.push_back(uuid);

    for (const UUID& uuid : m_uuid_set_copy) {
        CM->destroy(uuid);
    }
}
