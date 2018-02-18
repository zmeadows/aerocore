#pragma once

#include "aerocore.hpp"
#include "Base.hpp"
#include "QuadTree.hpp"

class MotionSystem final : public System {
    QuadTree* const m_quadTree = nullptr;

    struct HandleOffScreenBehaviorResult {
        bool skip_updating_kinematic_data = false;
        bool skip_updating_collision_data = false;
        bool to_be_destroyed = false;
    };

    HandleOffScreenBehaviorResult handle_offscreen_behavior(const UUID& uuid, CoreData& cd, const Extent& ext);

public:
    void run(float dt) final;

    MotionSystem(ComponentManager* const CM_, QuadTree* const quadTree) :
        System("Motion", CM_), m_quadTree(quadTree)
    {
        CM->subscribe<CoreData>(this);
    }
};
