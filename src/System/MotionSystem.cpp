#include "System/MotionSystem.hpp"

#include "Sprite.hpp"
#include "Entity.hpp"

MotionSystem::HandleOffScreenBehaviorResult
MotionSystem::handle_offscreen_behavior(const UUID& uuid, Entity& CD)
{
    auto res = HandleOffScreenBehaviorResult();
    const Extent& ext = CD.extent;

    auto CM = get_manager();

    auto& osb = CM->get<OffscreenBehavior>(uuid);

    //////////////
    // WRAPPING //
    //////////////

    if (std::holds_alternative<WrapOSB>(osb)) {

        if (ext.maxX > 100.f && CD.vel.x > 0.f) {
            CD.pos.x -= (ext.maxX + 100.f);
        }

        if (ext.minX < -100.f && CD.vel.x < 0.f) {
            CD.pos.x -= (ext.minX - 100.f);
        }

        if (ext.maxY > 100.f && CD.vel.y > 0.f) {
            CD.pos.y -= (ext.maxY + 100.f);
        }

        if (ext.minY < -100.f && CD.vel.y < 0.f) {
            CD.pos.y -= (ext.minY - 100.f);
        }
    }

    /////////////////
    // SINGLE-PASS //
    /////////////////

    else if (std::holds_alternative<SinglePassOSB>(osb)) {

        if (std::get<SinglePassOSB>(osb).found_onscreen) {
            res.to_be_destroyed = true;
            res.skip_updating_kinematic_data = true;
            res.skip_updating_collision_data = true;
        } else {
            res.skip_updating_collision_data = true;
        }
    }

    /////////////////
    // VALID-RANGE //
    /////////////////

    else if (std::holds_alternative<ValidRangeOSB>(osb)) {
        auto& vrosb = std::get<ValidRangeOSB>(osb);
        if (   CD.pos.x > vrosb.maxx
            || CD.pos.x < vrosb.minx
            || CD.pos.y > vrosb.maxy
            || CD.pos.y < vrosb.miny)
        {
            res.to_be_destroyed = true;
            res.skip_updating_kinematic_data = true;
            res.skip_updating_collision_data = true;
        }

    /////////////
    // NOTHING //
    /////////////

    }

    return res;
}

void MotionSystem::run(float dt) {
    UUIDSet toDestroy;

    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
        Entity& entity = CM->get<Entity>(uuid);

        const bool entity_is_offscreen = is_offscreen(entity);
        const bool entity_has_offscreen_behavior = CM->has<OffscreenBehavior>(uuid);

        HandleOffScreenBehaviorResult res;
        if (entity_is_offscreen && entity_has_offscreen_behavior)
            res = handle_offscreen_behavior(uuid, entity);

        if (entity_is_offscreen && !entity_has_offscreen_behavior) {
            res.to_be_destroyed = true;
            res.skip_updating_kinematic_data = true;
            res.skip_updating_collision_data = true;
        }

        if (res.to_be_destroyed) {
            toDestroy.insert(uuid);
            assert(res.skip_updating_kinematic_data && res.skip_updating_collision_data);
        }

        if (!entity_is_offscreen && entity_has_offscreen_behavior) {
            auto& osb = CM->get<OffscreenBehavior>(uuid);
            if (std::holds_alternative<SinglePassOSB>(osb)) {
                CM->remove<OffscreenBehavior>(uuid);
            }
        }

        if (!res.skip_updating_kinematic_data) {
            const float dx = dt * (entity.vel.x + 0.5f * entity.acc.x * dt);
            const float dy = dt * (entity.vel.y + 0.5f * entity.acc.y * dt);
            const float dtheta = entity.angvel * dt;

            entity.pos.x += dx;
            entity.pos.y += dy;
            entity.angle = rotate(entity.angle, dtheta);

            entity.vel.x += dt * (entity.acc.x - signum(entity.vel.x) * 0.3f);
            entity.vel.y += dt * (entity.acc.y - signum(entity.vel.y) * 0.3f);

            for (size_t i = 0; i < entity.local_vertices.size(); i++) {

                v2 local_vtx_copy = entity.local_vertices[i];
                local_vtx_copy.rotate(entity.angle);

                v2& global_vtx = entity.global_vertices[i];
                global_vtx.x = entity.pos.x + local_vtx_copy.x;
                global_vtx.y = entity.pos.y + local_vtx_copy.y;

                entity.extent.maxX = std::max(entity.extent.maxX, global_vtx.x);
                entity.extent.maxY = std::max(entity.extent.maxY, global_vtx.y);
                entity.extent.minX = std::min(entity.extent.minX, global_vtx.x);
                entity.extent.minY = std::min(entity.extent.minY, global_vtx.y);
            }
        }

        const bool entity_has_collision_data = CM->has<CollisionData>(uuid);

        if (!res.skip_updating_collision_data && entity_has_collision_data) {
            auto& coldat = CM->get<CollisionData>(uuid);

            if (!coldat.node) {
                coldat.node = get_quad_tree()->insert_entity(uuid, clip_to_screen(entity.extent));
            } else {
                coldat.node = coldat.node->update_entity(uuid, clip_to_screen(entity.extent));
            }
        }
    }

    for (const UUID uuid : toDestroy)
        destroy_entity(uuid);
}


