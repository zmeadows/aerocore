#pragma once

#include <array>
#include <memory>
#include <optional>
#include <set>


#include "Base.hpp"
#include "UUID.hpp"

struct QuadNode {
    const unsigned c_depth;
    const float c_x, c_y, c_w;
    bool m_hasChildren;
    UUIDSet m_containedUUIDs;

    QuadNode* m_parent = nullptr;
    std::array<std::unique_ptr<QuadNode>, 4> m_childNodes;

    bool is_in_node_boundary(const Extent& ext) const;
    void produce_children(const unsigned max_depth);
    void release_entity(const UUID& uuid) { m_containedUUIDs.erase(uuid); }
    bool contains_entity(const UUID& uuid) const;
    QuadNode* insert_entity(const UUID& uuid, const Extent& ext);
    QuadNode* update_entity(const UUID& uuid, const Extent& ext) {
        m_containedUUIDs.erase(uuid);
        return insert_entity(uuid,ext);
    }

    bool has_parent(const QuadNode* node) const;
    void retrieve(std::vector<UUID>& candidates, const UUID collider);
    void retrieve_from_parents(std::vector<UUID>& candidatess, const UUID collider);
    void retrieve_from_children(std::vector<UUID>& candidatess, const UUID collider);

    QuadNode(QuadNode* parent, const unsigned depth_, float _x, float _y, float _w) :
        c_depth(depth_), c_x(_x), c_y(_y), c_w(_w), m_hasChildren(false), m_parent(parent) {}

};


class QuadTree {
private:
    const unsigned c_maxDepth;

public:
    QuadNode* insert_entity(const UUID& uuid, const Extent& ext) { return m_top.insert_entity(uuid, ext); }
    QuadNode m_top;

    void retrieve(std::vector<UUID>& candidate, const Extent& ext);

    // std::vector<std::pair<UUID,UUID>> collision_candidates(void);

    QuadTree(void) = delete;

    QuadTree(unsigned max_depth);
};

