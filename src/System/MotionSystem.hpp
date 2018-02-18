#pragma once

#include "aerocore.hpp"
#include "Base.hpp"
#include "QuadTree.hpp"

class MotionSystem final : public System {
    void wrapEntity(const UUID& uuidA);

    QuadTree* const m_quadTree = nullptr;

public:
    void run(float dt) final;

    MotionSystem(ComponentManager* const CM_, QuadTree* const quadTree) :
        System("Motion", CM_), m_quadTree(quadTree)
    {
        CM->subscribe<CoreData>(this);
    }
};
