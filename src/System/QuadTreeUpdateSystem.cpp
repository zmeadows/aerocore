#include "System/QuadTreeUpdateSystem.hpp"

#include "Globals.hpp"

void run(QuadTreeUpdateSystem& self) {
    auto CM = get_manager();

    get_quad_tree()->reset();

    for (const UUID uuid : self.base.followed) {
        auto& entity = CM->get<Entity>(uuid);
        auto& cd = CM->get<CollisionData>(uuid);

        cd.node = get_quad_tree()->insert_entity(uuid, clip_to_screen(entity.extent));
    }
}
