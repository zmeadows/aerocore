#include "System/MotionSystem.hpp"

// #include "Sprite.hpp"
// #include "Entity.hpp"
//
// bool
// MotionSystem::handle_offscreen_behavior(Entity& entity)
// {
//     const Extent& ext = entity.extent;
//
//     auto& osb = entity.osb;
//
//     /////////////
//     // DEFAULT //
//     /////////////
//
//     if (osb.type == OffscreenBehavior::Type::DiesInstantly) {
//         return true;
//     }
//
//     //////////////
//     // WRAPPING //
//     //////////////
//
//     else if (osb.type == OffscreenBehavior::Type::Wraps) {
//         if (ext.maxX > 100.f && entity.vel.x > 0.f) {
//             entity.pos.x -= (ext.maxX + 100.f);
//         }
//
//         if (ext.minX < -100.f && entity.vel.x < 0.f) {
//             entity.pos.x -= (ext.minX - 100.f);
//         }
//
//         if (ext.maxY > 100.f && entity.vel.y > 0.f) {
//             entity.pos.y -= (ext.maxY + 100.f);
//         }
//
//         if (ext.minY < -100.f && entity.vel.y < 0.f) {
//             entity.pos.y -= (ext.minY - 100.f);
//         }
//
//         return false;
//     }
//
//     /////////////////
//     // SINGLE-PASS //
//     /////////////////
//
//     else if (osb.type == OffscreenBehavior::Type::SinglePassAllowed) {
//         return osb.SinglePassAllowed.already_found_onscreen;
//     }
//
//     /////////////////
//     // VALID-RANGE //
//     /////////////////
//
//     else if (osb.type == OffscreenBehavior::Type::ValidRange) {
//         if (  entity.pos.x > osb.ValidRange.maxX
//            || entity.pos.x < osb.ValidRange.minX
//            || entity.pos.y > osb.ValidRange.maxY
//            || entity.pos.y < osb.ValidRange.minY
//            ) return true;
//     }
//
//     //@TODO: use switch statement to avoid fall-through return
//     return true;
// }
//
// void MotionSystem::run(float dt) {
//     UUIDSet toDestroy;
//
//     static ComponentManager* CM = get_manager();
//
//     std::vector<UUID> friendly_entities;
//
//     for (const UUID uuid : m_followed) {
//         Entity& entity = CM->get<Entity>(uuid);
//
//         bool entity_should_be_destroyed = is_offscreen(entity) ? handle_offscreen_behavior(entity) : false;
//
//         if (entity_should_be_destroyed) {
//             toDestroy.insert(uuid);
//             continue;
//         }
//
//         //@OPTIMIZE: see if you can get LLVM to vectorize some of this
//
//         const float dx = dt * (entity.vel.x + 0.5f * entity.acc.x * dt);
//         const float dy = dt * (entity.vel.y + 0.5f * entity.acc.y * dt);
//         const float dtheta = entity.angvel * dt;
//
//         entity.pos.x += dx;
//         entity.pos.y += dy;
//         entity.angle = rotate(entity.angle, dtheta);
//
//         entity.vel.x += dt * (entity.acc.x - signum(entity.vel.x) * entity.drag);
//         entity.vel.y += dt * (entity.acc.y - signum(entity.vel.y) * entity.drag);
//
//         entity.extent.minX = std::numeric_limits<float>::max();
//         entity.extent.maxX = std::numeric_limits<float>::lowest();
//         entity.extent.minY = std::numeric_limits<float>::max();
//         entity.extent.maxY = std::numeric_limits<float>::lowest();
//
//         for (size_t i = 0; i < entity.local_vertices.count; i++) {
//             v2 local_vtx_copy = entity.local_vertices[i];
//             local_vtx_copy.rotate(entity.angle);
//
//             entity.global_vertices[i].x = entity.pos.x + local_vtx_copy.x;
//             entity.global_vertices[i].y = entity.pos.y + local_vtx_copy.y;
//
//             entity.extent.maxX = std::max(entity.extent.maxX, entity.global_vertices[i].x);
//             entity.extent.maxY = std::max(entity.extent.maxY, entity.global_vertices[i].y);
//             entity.extent.minX = std::min(entity.extent.minX, entity.global_vertices[i].x);
//             entity.extent.minY = std::min(entity.extent.minY, entity.global_vertices[i].y);
//         }
//
//         entity.node->m_containedUUIDs.erase(uuid);
//         entity.node = get_quad_tree()->insert_entity(uuid, clip_to_screen(entity.extent));
//
//         if (entity.friendly)
//             friendly_entities.push_back(uuid);
//     }
//
//     std::vector<UUID> collision_candidates;
//     collision_candidates.reserve(10);
//     for (const UUID& uuid : friendly_entities) {
//         const Entity& entity = CM->get<Entity>(uuid);
//         collision_candidates.clear();
//         entity.node->retrieve(collision_candidates, uuid);
//
//         for (UUID other_uuid : collision_candidates) {
//
//             assert(other_uuid != uuid);
//
//             const auto& other_entity = CM->get<Entity>(other_uuid);
//
//             if (entity.friendly != other_entity.friendly && overlaps(entity, other_entity)) {
//                 // toDestroy.insert(uuid);
//                 // toDestroy.insert(other_uuid);
//             }
//         }
//     }
//
//     for (const UUID uuid : toDestroy)
//         destroy_entity(uuid);
// }
//
//
