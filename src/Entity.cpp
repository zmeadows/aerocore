#include "Entity.hpp"

#include "Generator/Enemy.hpp"
#include "Sprite.hpp"
#include "Globals.hpp"

// void destroy_entity(const UUID& uuid) {
//     auto CM = get_manager();
//
//     auto& entity = CM->get<Entity>(uuid);
//
//     if (entity.node)
//             entity.node->m_containedUUIDs.erase(uuid);
//
//     play_sound(SoundEffect_Explosion);
//
//     CM->destroy(uuid);
// }

// void
// recompute_global_context(UUID uuid, Entity& entity)
// {
//     populate_global_vertices(entity.local_vertices, entity.global_vertices,
//                              entity.pos, entity.angle);
//     entity.extent = extent_of(entity.global_vertices);
//     //@TODO: apply clip_to_screen within quad tree insert function
//     entity.node = get_quad_tree()->insert_entity(uuid, clip_to_screen(entity.extent));
// }

v2 orientation_of(const Entity& entity) {
    return { std::cos(entity.angle), std::sin(entity.angle) };
}

bool is_offscreen(const Entity& cd) {
    return is_offscreen(cd.extent);
}

namespace {
    v2 normal_buffer[256];
}

bool
overlaps( const Entity& entityA
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

    for (uint_least8_t polyA = 0; polyA < cdA.poly_decomp.count; polyA++) {
        for (uint_least8_t polyB = 0; polyB < cdB.poly_decomp.count; polyB++) {

            bool polygon_candidates_separated = false;

            const PolygonRep shivA = nth_polygon(cdA.poly_decomp, polyA);
            const PolygonRep shivB = nth_polygon(cdB.poly_decomp, polyB);
            fill_polygon_normals(gvA, shivA, normal_buffer);


            //@TODO: factor this out
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

