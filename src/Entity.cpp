#include "Entity.hpp"

#include "Enemy.hpp"
#include "Globals.hpp"

void destroy_entity(const UUID& uuid) {
    auto CM = get_manager();

    if (CM->has<CollisionData>(uuid)) {
        auto& coldat = CM->get<CollisionData>(uuid);
        if (coldat.node) coldat.node->release_entity(uuid);

        if (CM->get<Entity>(uuid).type == EntityType::Asteroid)
            generateAsteroidShards(CM->get<Entity>(uuid), coldat.triangles);
    }

    CM->destroy(uuid);
}
