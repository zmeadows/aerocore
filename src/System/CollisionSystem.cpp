#include "System/CollisionSystem.hpp"

#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"
#include "Globals.hpp"

namespace {

v2 normal_buffer[256];

bool overlaps( const Entity& entityA
             , const Entity& entityB
             , const CollisionData& cdA
             , const CollisionData& cdB)
{
    const auto extA = entityA.extent;
    const auto extB = entityB.extent;

    // 1. currently collisions are not processed offscreen (could change)
    if (is_offscreen(extA) || is_offscreen(extB))
        return false;

    // 2. check if the AABB's overlap, since this offers an early exit in many cases.
    if (  extA.minX > extB.maxX || extB.minX > extB.maxX
       || extA.minY > extB.maxY || extB.minY > extB.maxY  ) return false;


    // 3. otherwise, we have to check the individual polygons

    const std::vector<v2> gvA = compute_global_vertices(cdA.local_vertices, entityA.pos, entityA.angle);
    const std::vector<v2> gvB = compute_global_vertices(cdB.local_vertices, entityB.pos, entityB.angle);

    for (uint_least8_t polyA = 0; polyA < cdA.poly_decomp->count; polyA++) {
        for (uint_least8_t polyB = 0; polyB < cdB.poly_decomp->count; polyB++) {

            bool polygon_candidates_separated = false;

            const PolygonRep shivA = nth_polygon(cdA.poly_decomp, polyA);
            const PolygonRep shivB = nth_polygon(cdB.poly_decomp, polyB);
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

void run(CollisionSystem& self) {
    auto CM = get_manager();

    for (const UUID friendly_uuid : self.base.followed) {
        const auto& cdA = CM->get<CollisionData>(friendly_uuid);
        const auto& entityA = CM->get<Entity>(friendly_uuid);

        cdA.node->retrieve(self.collision_candidates, friendly_uuid);

        for (const UUID enemy_uuid : self.collision_candidates) {
            const auto& cdB = CM->get<CollisionData>(enemy_uuid);
            const auto& entityB = CM->get<Entity>(enemy_uuid);

            const bool is_enemy_collider = !CM->has<FriendlyTag>(enemy_uuid);
            const bool enemy_not_already_destructing = !CM->has<DestructTag>(enemy_uuid); // hack?
            const bool friendly_is_bullet = CM->has<BulletTag>(friendly_uuid);
            const bool enemy_is_bullet = CM->has<BulletTag>(enemy_uuid);
            const bool friendly_is_invincible = CM->has<Invincibility>(friendly_uuid);

            if (is_enemy_collider
                && enemy_not_already_destructing
                && !(friendly_is_bullet && enemy_is_bullet)
                && overlaps(entityA, entityB, cdA, cdB))
            {
                if (friendly_is_bullet) {
                    CM->book<DestructTag>(friendly_uuid);
                    f32 damage_amount = CM->get<CollideDamage>(friendly_uuid).amount;
                    CM->book<DamageEvent>(enemy_uuid).amount = damage_amount;
                } else if (!friendly_is_invincible) {
                    if (enemy_is_bullet) CM->book<DestructTag>(enemy_uuid);
                    f32 damage_amount = CM->get<CollideDamage>(enemy_uuid).amount;
                    CM->book<DamageEvent>(friendly_uuid).amount = damage_amount;
                    CM->book<Invincibility>(friendly_uuid);
                }
            }
        }
    }
}
