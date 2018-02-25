#include "Generator/Player.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include "Entity.hpp"
#include "Sprite.hpp"

#include <random>

void generatePlayer() {

    ComponentManager* CM = get_manager();

    Entity& player = CM->book<Entity>(playerUUID());


    assign_iso_triangle_vertices(player, 5.f, 10.f);
    auto parse_result = parse_svg_path(asset_path("sprites/player/path").c_str());
    player.local_vertices = parse_result.vertices;
    player.sprite_offset = parse_result.sprite_offset;
    //recenter(player.local_vertices);
    // for (v2& vtx : player.local_vertices)
    //     vtx.scale(0.25f);
    // for (v2& vtx : player.local_vertices)
    //     vtx.rotate(PI/2.f);
    player.poly_decomp = decompose_local_vertices(player.local_vertices);
    recompute_global_context(playerUUID(), player);
    assert(player.node);

    player.sprite = GPU_LoadImage(asset_path("sprites/player/player.png").c_str());

    player.osb.type = OffscreenBehavior::Type::Wraps;

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
}
