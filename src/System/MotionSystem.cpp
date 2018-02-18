#include "System/MotionSystem.hpp"

#include "Sprite.hpp"
#include "Entity.hpp"

MotionSystem::HandleOffScreenBehaviorResult
MotionSystem::handle_offscreen_behavior(const UUID& uuid, CoreData& CD, const Extent& ext)
{
    auto res = HandleOffScreenBehaviorResult();

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

    for (const UUID uuid : m_followed) {
        CoreData& CD = CM->get<CoreData>(uuid);

        v2& pos = CD.pos;
        v2& vel = CD.vel;
        const std::vector<v2>& vertices = CD.vertices;
        float& angle = CD.angle;

        const Extent ext = extent_at(CD);
        const bool entity_is_offscreen = is_offscreen(CD);
        const bool entity_has_offscreen_behavior = CM->has<OffscreenBehavior>(uuid);

        HandleOffScreenBehaviorResult res;
        if (entity_is_offscreen && entity_has_offscreen_behavior)
            res = handle_offscreen_behavior(uuid, CD, ext);

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
            v2& acc = CD.acc;

            pos.x += dt * (vel.x + 0.5f * acc.x * dt);
            pos.y += dt * (vel.y + 0.5f * acc.x * dt);

            vel.x += dt * (acc.x - signum(vel.x) * 0.4f);
            vel.y += dt * (acc.y - signum(vel.y) * 0.4f);

            rotate(angle, CD.angvel * dt);
        }

        const bool entity_has_collision_data = CM->has<CollisionData>(uuid);

        if (!res.skip_updating_collision_data && entity_has_collision_data) {
            auto& coldat = CM->get<CollisionData>(uuid);

            if (!coldat.node) {
                coldat.node = m_quadTree->insert_entity(uuid, clip_to_screen(ext));
            } else {
                coldat.node = coldat.node->update_entity(uuid, ext);
                // if (!coldat.node) {
                //     coldat.node = m_quadTree->insert_entity(uuid, clip_to_screen(ext));
                // }
            }
        }
    }

    for (const UUID uuid : toDestroy)
        destroy_entity(CM, uuid);
}


