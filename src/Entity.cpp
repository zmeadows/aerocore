#include "Entity.hpp"

#include "Enemy.hpp"
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

//@FIXME: This is still inefficient. Move individual triangles into quad tree
//and only test surface normals from each pair of triangles.
bool
overlaps( const CollisionData& entityA
        , const CollisionData& entityB
        , const Extent extA
        , const Extent extB)
{
    // 1. currently collisions are not processed offscreen (could change)
    if (is_offscreen(extA) || is_offscreen(extB))
        return false;

    // 2. check if the AABB's overlap, since this offers an early exit in many cases.
    if (  extA.minX > extB.maxX || extB.minX > extB.maxX
       || extA.minY > extB.maxY || extB.minY > extB.maxY  ) return false;


    // 3. otherwise, we have to check the individual polygons
    v2 normals[256];

    for (uint_least8_t polyA = 0; polyA < entityA.poly_decomp.count; polyA++) {
        for (uint_least8_t polyB = 0; polyB < entityB.poly_decomp.count; polyB++) {

            bool polygon_candidates_separated = false;

            const PolygonRep shivA = nth_polygon(entityA.poly_decomp, polyA);
            const PolygonRep shivB = nth_polygon(entityB.poly_decomp, polyB);
            fill_polygon_normals(entityA.global_vertices, shivA, normals);


            //@TODO: factor this out
            for (auto nidxA = 0; nidxA < shivA.count; nidxA++) {
                const AxisProjection projA = project_on(entityA.global_vertices, shivA, normals[nidxA]);
                const AxisProjection projB = project_on(entityB.global_vertices, shivB, normals[nidxA]);
                if (!(projA.max >= projB.min && projB.max >= projA.min)) {
                    //@TODO: at this point, swap separating axis to front of polygon decomp to save time next iteration.
                    polygon_candidates_separated = true;
                    break;
                }
            }

            if (polygon_candidates_separated)
                continue;

            fill_polygon_normals(entityB.global_vertices, shivB, normals);

            for (auto nidxB = 0; nidxB < shivB.count; nidxB++) {
                const AxisProjection projA = project_on(entityA.global_vertices, shivA, normals[nidxB]);
                const AxisProjection projB = project_on(entityB.global_vertices, shivB, normals[nidxB]);
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

