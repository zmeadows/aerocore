#include "System/CollisionSystem.hpp"

#include "Component/CollisionData.hpp"
#include "Component/Common.hpp"
#include "Engine/ComponentManager.hpp"
#include "LocalVertexBuffer.hpp"
#include "PolygonDecomposition.hpp"
#include "QuadTree.hpp"

namespace {

bool overlaps( const Entity& friendly_entity
             , const Entity& enemy_entity
             , const CollisionData& friendly_collision_data
             , const CollisionData& enemy_collision_data)
{
    const auto extA = friendly_entity.extent;
    const auto extB = enemy_entity.extent;

    // BUGFIX: make this a SmallVector to prevent overflow
    v2 normal_buffer[256];

    // 1. currently collisions are not processed offscreen (could change)
    if (is_offscreen(extA) || is_offscreen(extB))
        return false;

    // 2. check if the AABB's overlap, since this offers an early exit in many cases.
    if (  extA.minX > extB.maxX || extB.minX > extB.maxX
       || extA.minY > extB.maxY || extB.minY > extB.maxY  ) return false;


    // 3. otherwise, we have to check the individual polygons
    DynamicArray<v2> gvA = compute_global_vertices(friendly_collision_data.local_vertices, friendly_entity.pos, friendly_entity.angle);
    DynamicArray<v2> gvB = compute_global_vertices(enemy_collision_data.local_vertices, enemy_entity.pos, enemy_entity.angle);

    Defer( gvA.deallocate(); gvB.deallocate(); );

    for (uint_least8_t polyA = 0; polyA < friendly_collision_data.poly_decomp->count; polyA++) {
        for (uint_least8_t polyB = 0; polyB < enemy_collision_data.poly_decomp->count; polyB++) {

            bool polygon_candidates_separated = false;

            const PolygonRep shivA = nth_polygon(friendly_collision_data.poly_decomp, polyA);
            const PolygonRep shivB = nth_polygon(enemy_collision_data.poly_decomp, polyB);
            fill_polygon_normals(gvA, shivA, normal_buffer);

            for (auto nidxA = 0; nidxA < shivA.count; nidxA++) {
                const AxisProjection projA = project_on(gvA, shivA, normal_buffer[nidxA]);
                const AxisProjection projB = project_on(gvB, shivB, normal_buffer[nidxA]);
                if (!(projA.max >= projB.min && projB.max >= projA.min)) {
                    //@TODO: at this point, swap separating axis to front of polygon decomp to save time next iteration.
                    polygon_candidates_separated = true;
                    break;
                }
            }

            if (polygon_candidates_separated)
                continue;

            fill_polygon_normals(gvB, shivB, normal_buffer);

            for (auto nidxB = 0; nidxB < shivB.count; nidxB++) {
                const AxisProjection projA = project_on(gvA, shivA, normal_buffer[nidxB]);
                const AxisProjection projB = project_on(gvB, shivB, normal_buffer[nidxB]);
                if (!(projA.max >= projB.min && projB.max >= projA.min)) {
                    polygon_candidates_separated = true;
                    break;
                }
            }


            if (!polygon_candidates_separated) return true;
        }
    }

    return false;
}

}

void CollisionSystem::pre(ComponentManager*) {
    m_collisions.clear();
}

void CollisionSystem::post(ComponentManager* CM) {
    for (const auto collision : m_collisions) {
         const bool friendly_is_bullet = CM->has<BulletTag>(collision.friendly);
         const bool enemy_is_bullet = CM->has<BulletTag>(collision.enemy);

         if (friendly_is_bullet) {
             CM->book<DestructTag>(collision.friendly);
             const f32 damage_amount = CM->get<CollideDamage>(collision.friendly).amount;
             CM->book<DamageEvent>(collision.enemy).amount = damage_amount;
         } else if (!CM->has<Invincibility>(collision.friendly)) {
             // player was hit either by an enemy bullet or by the enemy itself
             if (enemy_is_bullet) CM->book<DestructTag>(collision.enemy);
             const f32 damage_amount = CM->get<CollideDamage>(collision.enemy).amount;
             CM->book<DamageEvent>(collision.friendly).amount = damage_amount;
             CM->book<Invincibility>(collision.friendly);
         }
    }
}

void CollisionSystem::par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt) {
    //TODO: make small set
    static thread_local DynamicArray<Collision> thread_collisions_detected;
    thread_collisions_detected.clear();

    //TODO: make small array
    static thread_local DynamicArray<UUID> thread_collision_candidates;

    for (const UUID friendly_uuid : entities) {
        if (CM->has<Invincibility>(friendly_uuid)) {
            continue;
        }

        const auto& friendly_collision_data = CM->get<CollisionData>(friendly_uuid);
        const auto& friendly_entity = CM->get<Entity>(friendly_uuid);

        thread_collision_candidates.clear();
        friendly_collision_data.node->retrieve(&thread_collision_candidates, friendly_uuid);

        for (const UUID enemy_uuid : thread_collision_candidates) {
            if (CM->has<FriendlyTag>(enemy_uuid)) {
                // not actually an enemy, so just pass
                continue;
            }

            if (CM->has<BulletTag>(friendly_uuid) && CM->has<BulletTag>(enemy_uuid)) {
                // bullets don't hit each other
                continue;
            }

            const auto& enemy_collision_data = CM->get<CollisionData>(enemy_uuid);
            const auto& enemy_entity = CM->get<Entity>(enemy_uuid);

            if (overlaps(friendly_entity, enemy_entity, friendly_collision_data, enemy_collision_data)) {

                thread_collisions_detected.append(Collision ({friendly_uuid, enemy_uuid}));
            }
        }
    }

    // now add the detected collisions from this thread into the system buffer
    std::scoped_lock<std::mutex> lock(m_mutex);
    for (auto c : thread_collisions_detected) {
        m_collisions.append(c);
    }
}
