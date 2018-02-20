#include "System/AsteroidShardSystem.hpp"

#include "Entity.hpp"

void AsteroidShardSystem::run(float dt) {
    std::vector<UUID> to_destroy;

    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto& shard = CM->get<AsteroidShardData>(uuid);
        Uint8 new_alpha = shard_tick(shard, dt);
        if (new_alpha == 0) {
            to_destroy.push_back(uuid);
        } else {
            CM->get<Entity>(uuid).color.a = new_alpha;
        }
    }

    for (const UUID& uuid : to_destroy)
        destroy_entity(uuid);
}

