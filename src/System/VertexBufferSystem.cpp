#include "System/VertexBufferSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"

VertexBufferSystem::VertexBufferSystem(void) : System("VertexBuffer")
{
    get_manager()->subscribe<Entity, CollisionData>(this);
}

//TODO: rename system to QuadTreeUpdateSystem
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
