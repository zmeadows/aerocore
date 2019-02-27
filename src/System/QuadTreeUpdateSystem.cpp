#include "System/QuadTreeUpdateSystem.hpp"

#include "Engine/ComponentManager.hpp"
#include "QuadTree.hpp"

void QuadTreeUpdateSystem::pre(ComponentManager*) {
    m_node_assocs.clear();
    QT->reset();
}

void QuadTreeUpdateSystem::par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt) {
    static thread_local auto thread_node_assocs = DynamicArray<NodeAssociation>::init(256);
    thread_node_assocs.clear();

    for (const UUID uuid : entities) {
        auto& entity = CM->get<Entity>(uuid);
        auto& cd = CM->get<CollisionData>(uuid);
        //TODO: remove this const_cast
        thread_node_assocs.append(NodeAssociation( {uuid, const_cast<QuadNode*>(QT->find_node(clip_to_screen(entity.extent))) }));
    }

    std::scoped_lock<std::mutex> lock(m_mutex);
    for (const auto& a : thread_node_assocs) {
        m_node_assocs.append(a);
    }
}

void QuadTreeUpdateSystem::post(ComponentManager* CM) {
    assert(m_node_assocs.size() == followed.size());
    for (const auto& a : m_node_assocs) {
        //TODO: This nullptr check has to do with offscreen behavior not immediately destroy an entity,
        //but rather booking a DestroyTag. Is there a better way to handle it?
        if (a.node != nullptr) {
            a.node->m_containedUUIDs.append(a.uuid);
            CM->get<CollisionData>(a.uuid).node = a.node;
        }
    }
}
