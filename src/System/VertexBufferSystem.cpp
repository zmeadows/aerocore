#include "System/VertexBufferSystem.hpp"

#include "Entity.hpp"

//TODO: move functionality into collision system
void VertexBufferSystem::run(float) {
    ComponentManager* CM = get_manager();

    get_quad_tree()->reset();

    for (const UUID uuid : m_followed) {
        auto& entity = CM->get<Entity>(uuid);
        auto& cd = CM->get<CollisionData>(uuid);

        // for (size_t i = 0; i < cd.local_vertices.count; i++) {
        //     v2 rotated_copy = cd.local_vertices[i].rotated(entity.angle);
        //     cd.global_vertices[i] = rotated_copy + entity.pos;
        // }

        cd.node = get_quad_tree()->insert_entity(uuid, clip_to_screen(entity.extent));
    }
}
