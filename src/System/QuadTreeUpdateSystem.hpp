#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"
#include <mutex>

class QuadTree;

struct QuadTreeUpdateSystem final : SimpleParallelSystem {
    QuadTree* QT;
    QuadTreeUpdateSystem(QuadTree* _QT) : QT(_QT), SimpleParallelSystem("QuadTreeUpdate") {
        m_node_assocs.reserve(1024);
    }
    void pre(ComponentManager*) final;
    void par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt);
    void post(ComponentManager*) final;

    struct NodeAssociation {
        UUID uuid;
        QuadNode* node = nullptr;
    };

    std::mutex m_mutex;
    DynamicArray<NodeAssociation> m_node_assocs;
    SUBSCRIBE(Entity, CollisionData);
};

