#pragma once

#include "InputManager.hpp"

#include "Entity/Player.hpp"
#include "Entity/Twister.hpp"

#include "Engine/ComponentManager.hpp"
#include "QuadTree.hpp"
#include "AudioContext.hpp"
#include "SpriteCache.hpp"
#include "GraphicsContext.hpp"

#include "System/BulletStreamSystem.hpp"
#include "System/CollisionSystem.hpp"
#include "System/DamageSystem.hpp"
#include "System/DestructSystem.hpp"
#include "System/DrawCollisionGeometrySystem.hpp"
#include "System/DrawSystem.hpp"
#include "System/EulerRotationSystem.hpp"
#include "System/EulerTranslationSystem.hpp"
#include "System/InvincibilitySystem.hpp"
#include "System/LinearPathSystem.hpp"
#include "System/OffscreenBehaviorSystem.hpp"
#include "System/PauseSystem.hpp"
#include "System/PositionUpdateSystem.hpp"
#include "System/QuadTreeUpdateSystem.hpp"
#include "System/RotationUpdateSystem.hpp"
#include "System/StateTransitionCleanupSystem.hpp"
#include "System/TranslationSplineSystem.hpp"


struct Game {
    ComponentManager CM;
    QuadTree QT;
    AudioContext AC;
    GraphicsContext GC;
    SpriteCache SC;

    u64 m_preSystemRunTicks;
    u64 m_postSystemRunTicks;
    u64 m_frames_elapsed;
    f32 time_elapsed;
    PlayerInput input;
    PlayerState state;

    EulerTranslationSystem         euler_translation_sys;
    EulerRotationSystem            euler_rotation_sys;
    TranslationSplineSystem        translation_spline_sys;
    OffscreenBehaviorSystem        offscreen_behavior_sys;
    PositionUpdateSystem           position_update_sys;
    RotationUpdateSystem           rotation_update_sys;
    PauseSystem                    pause_sys;
    Twister::StateMachineSystem    twister_state_sys;
    StateTransitionCleanupSystem   state_transition_cleanup_sys;
    BulletStreamSystem             bullet_stream_sys;
    InvincibilitySystem            invincibility_sys;
    QuadTreeUpdateSystem           quadtree_update_sys;
    CollisionSystem                collision_sys;
    DamageSystem                   damage_sys;
    DestructSystem                 destruct_sys;
    DrawSystem                     draw_sys;
    DrawCollisionGeometrySystem    draw_collision_geometry_sys;

    bool poll_input(void);
    bool tick(void);

    u64 last_asteroid_time;

public:

    void go(void) {
        while (!tick())
            continue;
    }

    Game(void);
    ~Game(void);

    Game(const Game&)            = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&)                 = delete;
    Game& operator=(Game&&)      = delete;
};
