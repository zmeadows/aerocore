#include "Generator.hpp"
#include "Generator/Player.hpp"
#include "Geometry.hpp"

#include <random>

UUID generatePlayer(ComponentManager* CM, QuadTree* quadTree) {

    CoreData& CD = CM->book<CoreData>(playerUUID());

    CM->book<Alliance>(playerUUID(), Alliance::Friend);

    CD.wraps = true;

    CD.vertices = make_iso_triangle_vertices(7.0, 12.0);
    assert(is_convex(CD.vertices));

    auto& coldat = CM->book<CollisionData>(playerUUID());
    coldat.friendly = true;
    coldat.triangles = triangulate(CD.vertices);
    coldat.normals = SurfaceNormalSet(coldat.triangles);
    coldat.node = quadTree->insert_entity(playerUUID(), extent_at(CD));

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
            const auto& playerCD = CM->get<CoreData>(playerUUID());
            const v2& pacc = playerCD.acc;
            if (std::fabs(pacc.x) > 0 || std::fabs(pacc.y) > 0) {
                for (size_t i = 0; i < 4; i++) {
                const v2& ppos = playerCD.pos;
                const v2& pvel = playerCD.vel;
                const float pangle = playerCD.angle;

                UUID partUUID;

                const float r1 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
                const float r2 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
                const float r3 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
                const float r4 = static_cast<float>(rand()) / static_cast <float>(RAND_MAX);

                CoreData& partCD = CM->book<CoreData>(partUUID);
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
