#include "Generator.hpp"
#include "Generator/Player.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"

#include <random>

UUID generatePlayer() {

    auto CM = get_manager();

    Entity& CD = CM->book<Entity>(playerUUID());

    CM->book<Alliance>(playerUUID(), Alliance::Friend);

    // CD.vertices = make_iso_triangle_vertices(7.0, 12.0);
    // assert(is_convex(CD.vertices));

    CD.vertices = {
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

    for (v2& vtx : CD.vertices)
        vtx.scale(3.f);

    auto& coldat = CM->book<CollisionData>(playerUUID());
    coldat.friendly = true;
    coldat.triangles = decompose(CD.vertices);
    coldat.normals = SurfaceNormalSet(coldat.triangles);
    coldat.node = get_quad_tree()->insert_entity(playerUUID(), extent_of(CD));

    assert(coldat.node);

    CD.color.r = 255;
    CD.color.g = 255;
    CD.color.b = 255;
    CD.color.a = 255;

    // CM->book<BoundingSurface*>(playerUUID(), spr->buildBoundingSurface());

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

    auto pgen = [CM](void) -> void {

        if (static_cast<float>(rand()) / static_cast <float>(RAND_MAX) < 9999.f) {
            const auto& playerCD = CM->get<Entity>(playerUUID());
            const v2& pacc = playerCD.acc;
            if (std::fabs(pacc.x) > 0 || std::fabs(pacc.y) > 0) {
                for (size_t i = 0; i < 5; i++) {
                const v2& ppos = playerCD.pos;
                const v2& pvel = playerCD.vel;
                const float pangle = playerCD.angle;

                UUID partUUID;

                const float r1 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
                const float r2 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
                const float r3 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
                const float r4 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);

                Entity& partCD = CM->book<Entity>(partUUID);
                partCD.pos.x = ppos.x + 6.f * std::sin(pangle) + 1.f * r1;
                partCD.pos.y = ppos.y - 6.f * std::cos(pangle) + 1.f * r2;

                partCD.vel.x =  -50.f * std::sin(-pangle) + 0.3f * pvel.x + 50.f * (0.5f - r3) - 3.f * signum(pacc.x);
                partCD.vel.y =  -50.f * std::cos(-pangle) + 0.3f * pvel.y + 50.f * (0.5f - r4) - 3.f * signum(pacc.y);

                CM->book<Alliance>(partUUID, Alliance::Friend);
                auto& dt = CM->book<DeathTimer>(partUUID);
                dt.value = 2.0;

                partCD.vertices = make_square_vertices(0.5);
                partCD.color.r = 255;
                partCD.color.g = 255;
                partCD.color.b = 255;
                partCD.color.a = 10;

                // auto sprUpd = [](Sprite* thisSpr) -> void {
                //     SimplePolygonSprite* spr = static_cast<SimplePolygonSprite*>(thisSpr);
                //     spr->rgba.a = std::max(0, spr->rgba.a - 2);
                // };

                // CM->book<SpriteUpdator>(partUUID, {sprUpd});
                }
            }
        }
    };

    CM->book<ParticleGenerator>(playerUUID(), pgen);

    return playerUUID();
}
