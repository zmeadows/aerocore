#include "System/VertexBufferSystem.hpp"

#include "Entity.hpp"

void VertexBufferSystem::run(float dt) {
    static ComponentManager* CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto& entity = CM->get<Entity>(uuid);
        auto& cd = CM->get<CollisionData>(uuid);

        for (size_t i = 0; i < cd.local_vertices.count; i++)
            cd.global_vertices[i] = cd.local_vertices[i].rotated(entity.angle) + entity.pos;

        cd.node->m_containedUUIDs.erase(uuid);
        cd.node = get_quad_tree()->insert_entity(uuid, clip_to_screen(entity.extent));
    }
}
