#include "System/CollisionSystem.hpp"

#include "Entity.hpp"

void CollisionSystem::run(float) {
    //@FIXME: what to use besides a set?
    std::set<std::pair<UUID, UUID>> collisions;

    auto CM = get_manager();

    for (UUID uuidA : m_followed) {
        const auto& entityA = CM->get<Entity>(uuidA);

        if (!entityA.friendly || !entityA.node)
            continue;

        std::vector<UUID> collision_candidates;
        entityA.node->retrieve(collision_candidates, uuidA);

        //@FIXME: just remove the UUID wrapper and do typedef uint32_t UUID
        for (UUID uuidB : collision_candidates) {

            assert(uuidB != uuidA);

            const auto& entityB = CM->get<Entity>(uuidB);

            if (entityA.friendly != entityB.friendly && overlaps(entityA, entityB)) {
                collisions.insert(std::make_pair(uuidA, uuidB));
                break;
            }
        }
    }

    //@TODO: resolve cases where one entity collides with two other entities one same frame.
    // for (const auto& p : collisions) {
    //     destroy_entity(p.first);
    //     destroy_entity(p.second);
    // }
}

