#include "Bullet/Bullet.hpp"
#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"
#include "Engine/UUID.hpp"
#include "Engine/ComponentManager.hpp"
#include "Entity/Player.hpp"
#include "Geometry.hpp"
#include "SpriteCache.hpp"
#include "InputManager.hpp"

const UUID playerUUID(void) {
    static UUID id;
    return id;
}

void generate_player(ComponentManager* CM, SpriteCache* SC) {
    Entity& player = CM->book<Entity>(playerUUID());
    player.angle = PI/2.0;

    CM->book<EulerTranslation>(playerUUID());
    CM->book<EulerRotation>(playerUUID());
    CM->book<FriendlyTag>(playerUUID());

    CM->book<Health>(playerUUID());

    SC->attach_sprite_to_uuid(CM, playerUUID(), "player_idle");

    auto& osb = CM->book<OffscreenBehavior>(playerUUID());
    osb.type = OffscreenBehavior::Type::Wraps;
}

void generate_player_bullet(ComponentManager* CM, AudioContext* AC, SpriteCache* SC, v2 bullet_direction) {
    bullet_direction = bullet_direction.normalized();

    const auto& entity = CM->get<Entity>(playerUUID());
    const v2 bullet_pos = entity.pos + 4.0 * bullet_direction;
    const v2 bullet_vel = 200.f * bullet_direction;

    generate_bullet(CM, AC, SC, PLAYER_BULLET, bullet_pos, bullet_vel, true);
}

void tick_player(ComponentManager* CM,
                 AudioContext* AC,
                 SpriteCache* SC,
                 PlayerInput* input,
                 PlayerState* state,
                 f32 dt) {
    if (state->shot_countdown > 0.f)
        state->shot_countdown -= dt;

    const bool player_wants_to_shoot = input->joystick_active[RIGHT_JOYSTICK];

    if (player_wants_to_shoot && state->shot_countdown <= 0.f) {
        generate_player_bullet(CM, AC, SC, input->joystick_state[RIGHT_JOYSTICK]);
        state->shot_countdown += state->shot_delay;
    }

    auto& kin = CM->get<EulerTranslation>(playerUUID());
    if (input->joystick_active[LEFT_JOYSTICK]) {
        kin.vel = 90.f * input->joystick_state[LEFT_JOYSTICK];
    } else {
        kin.vel = { 0.f, 0.f };
    }
}
