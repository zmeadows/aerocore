#include "System/QuadTreeUpdateSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"

QuadTreeUpdateSystem::QuadTreeUpdateSystem(void) : System("QuadTreeUpdateSystem")
{
    get_manager()->subscribe<Entity, CollisionData>(this);
}

//TODO: rename system to QuadTreeUpdateSystem
void QuadTreeUpdateSystem::run(float) {

    ComponentManager* CM = get_manager();

    get_quad_tree()->reset();

    for (const UUID uuid : m_followed) {
        auto& entity = CM->get<Entity>(uuid);
        auto& cd = CM->get<CollisionData>(uuid);

        cd.node = get_quad_tree()->insert_entity(uuid, clip_to_screen(entity.extent));
    }
}
