#include "Generator.hpp"
#include "Generator/Player.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include "Entity.hpp"

#include <random>

UUID generatePlayer() {

    auto CM = get_manager();

    Entity& player = CM->book<Entity>(playerUUID());
    player.type = EntityType::Player;

    player.local_vertices = {
        { -1.f, -2.f },
        { -2.f, -1.f },
        { -2.f, 0.f },
        { -1.f, 3.f },
        { -1.f, 0.f },
        { 1.f, 0.f },
        { 1.f, 3.f },
        { 2.f, 0.f },
        { 2.f, -1.f },
        { 1.f, -2.f }
    };

    for (v2& vtx : player.local_vertices)
        vtx.scale(2.f);

    reset_global_vertices(player);
    player.extent = extent_of(player);

    auto& coldat = CM->book<CollisionData>(playerUUID(), player);
    coldat.friendly = true;
    coldat.node = get_quad_tree()->insert_entity(playerUUID(), player.extent);

    assert(coldat.node);

    player.color.r = 255;
    player.color.g = 255;
    player.color.b = 255;
    player.color.a = 255;

    // auto sprUpd = [CM](Sprite* thisSpr) -> void {
    //     SimplePolygonSprite* spr = static_cast<SimplePolygonSprite*>(thisSpr);

    //     auto pacc = CM->get<Acceleration>(playerUUID());
    //     if (std::fabs(pacc.x) > 0 || std::fabs(pacc.y) > 0) {
    //         spr->rgba.r = std::min(255, spr->rgba.r + 3);
    //         spr->rgba.g = std::min(255, spr->rgba.r + 3);
    //         spr->rgba.b = std::min(255, spr->rgba.r + 3);
    //     } else {
    //         spr->rgba.r = std::max(100, spr->rgba.r - 3);
    //         spr->rgba.g = std::max(100, spr->rgba.r - 3);
    //         spr->rgba.b = std::max(100, spr->rgba.r - 3);
    //     }
    // };

    // CM->book<SpriteUpdator>(playerUUID(), {sprUpd});

    auto& osb = CM->book<OffscreenBehavior>(playerUUID());
    osb = WrapOSB();

    auto& sd = CM->book<ShotDelay>(playerUUID());
    sd.delay = 1000.f;

    // auto pgen = [CM](void) -> void {

    //     if (static_cast<float>(rand()) / static_cast <float>(RAND_MAX) < 9999.f) {
    //         const auto& playerCD = CM->get<Entity>(playerUUID());
    //         const v2& pacc = playerCD.acc;
    //         if (std::fabs(pacc.x) > 0 || std::fabs(pacc.y) > 0) {
    //             for (size_t i = 0; i < 5; i++) {
    //             const v2& ppos = playerCD.pos;
    //             const v2& pvel = playerCD.vel;
    //             const float pangle = playerCD.angle;

    //             UUID partUUID;

    //             const float r1 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
    //             const float r2 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
    //             const float r3 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
    //             const float r4 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);

    //             Entity& partCD = CM->book<Entity>(partUUID);
    //             partCD.pos.x = ppos.x + 6.f * std::sin(pangle) + 1.f * r1;
    //             partCD.pos.y = ppos.y - 6.f * std::cos(pangle) + 1.f * r2;

    //             partCD.vel.x =  -50.f * std::sin(-pangle) + 0.3f * pvel.x + 50.f * (0.5f - r3) - 3.f * signum(pacc.x);
    //             partCD.vel.y =  -50.f * std::cos(-pangle) + 0.3f * pvel.y + 50.f * (0.5f - r4) - 3.f * signum(pacc.y);

    //             auto& dt = CM->book<DeathTimer>(partUUID);
    //             dt.value = 2.0;

    //             partCD.vertices = make_square_vertices(0.5);
    //             partCD.color.r = 255;
    //             partCD.color.g = 255;
    //             partCD.color.b = 255;
    //             partCD.color.a = 10;

    //             // auto sprUpd = [](Sprite* thisSpr) -> void {
    //             //     SimplePolygonSprite* spr = static_cast<SimplePolygonSprite*>(thisSpr);
    //             //     spr->rgba.a = std::max(0, spr->rgba.a - 2);
    //             // };

    //             // CM->book<SpriteUpdator>(partUUID, {sprUpd});
    //             }
    //         }
    //     }
    // };

    // CM->book<ParticleGenerator>(playerUUID(), pgen);

    return playerUUID();
}
