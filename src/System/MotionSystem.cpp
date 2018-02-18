#include "System/MotionSystem.hpp"

#include "Sprite.hpp"

void MotionSystem::run(float dt) {
    UUIDSet toDestroy;

    for (const UUID uuid : m_followed) {
        CoreData& CD = CM->get<CoreData>(uuid);
        v2& pos = CD.pos;
        v2& vel = CD.vel;
        const std::vector<v2>& vertices = CD.vertices;
        float& angle = CD.angle;

        bool skipUpdating = false;
        bool skipUpdatingCollisionData = false;

        const bool was_offscreen = is_offscreen(vertices, pos, angle);

        if (was_offscreen) {

            if (CM->has<OffscreenBehavior>(uuid)) {
                const Extent ext = extent_at(vertices, pos, angle);

                auto& osb = CM->get<OffscreenBehavior>(uuid);

                //////////////
                // WRAPPING //
                //////////////

                if (std::holds_alternative<WrapOSB>(osb)) {

                    if (ext.maxX > 100.f && vel.x > 0.f) {
                        pos.x -= (ext.maxX + 100.f);
                    }

                    if (ext.minX < -100.f && vel.x < 0.f) {
                        pos.x -= (ext.minX - 100.f);
                    }

                    if (ext.maxY > 100.f && vel.y > 0.f) {
                        pos.y -= (ext.maxY + 100.f);
                    }

                    if (ext.minY < -100.f && vel.y < 0.f) {
                        pos.y -= (ext.minY - 100.f);
                    }
                }

                /////////////////
                // SINGLE-PASS //
                /////////////////

                else if (std::holds_alternative<SinglePassOSB>(osb)) {
                    if (std::get<SinglePassOSB>(osb).found_onscreen) {
                        toDestroy.insert(uuid);
                        skipUpdating = true;
                    } else {
                        skipUpdatingCollisionData = true;
                    }
                }

                /////////////////
                // VALID-RANGE //
                /////////////////

                else if (std::holds_alternative<ValidRangeOSB>(osb)) {
                    auto& vrosb = std::get<ValidRangeOSB>(osb);
                    if (   pos.x > vrosb.maxx
                        || pos.x < vrosb.minx
                        || pos.y > vrosb.maxy
                        || pos.y < vrosb.miny)
                    {
                        toDestroy.insert(uuid);
                        skipUpdating = true;
                    }
                }
            } else {  // offscreen but no specified behavior, so destroy.
                toDestroy.insert(uuid);
                skipUpdating = true;
            }

        }
        // entity found on screen
        else if (CM->has<OffscreenBehavior>(uuid)){
            auto& osb = CM->get<OffscreenBehavior>(uuid);
            if (std::holds_alternative<SinglePassOSB>(osb)) {
                std::get<SinglePassOSB>(osb).found_onscreen = true;
            }
        }

        if (!skipUpdating) {
            v2& acc = CD.acc;

            pos.x += dt * (vel.x + 0.5f * acc.x * dt);
            pos.y += dt * (vel.y + 0.5f * acc.x * dt);

            vel.x += dt * (acc.x - signum(vel.x) * 0.4f);
            vel.y += dt * (acc.y - signum(vel.y) * 0.4f);

            rotate(angle, CD.angvel * dt);

            // FIXME
            if (!skipUpdatingCollisionData && CM->has<CollisionData>(uuid)) {
                const Extent ext = extent_at(vertices, pos, angle);
                auto& coldat = CM->get<CollisionData>(uuid);

                if (!coldat.node) {
                    coldat.node = m_quadTree->insert_entity(uuid, clip_to_screen(ext));
                } else {
                    coldat.node = coldat.node->update_entity(uuid, ext);
                    if (!coldat.node) {
                        coldat.node = m_quadTree->insert_entity(uuid, clip_to_screen(ext));
                    }
                }
            }
        }
    }

    for (const UUID uuid : toDestroy) {
        if (CM->has<CollisionData>(uuid)) {
            auto& coldat = CM->get<CollisionData>(uuid);
            if (coldat.node)
                coldat.node->release_entity(uuid);
        }

        CM->destroy(uuid);
    }
}


