#pragma once

#include <array>
#include <memory>
#include <set>

#include "Engine/UUID.hpp"
#include "Extent.hpp"
#include "unstd/DynamicArray.hpp"

struct QuadNode {
    const unsigned c_depth;
    const float c_x, c_y, c_w;
    bool m_hasChildren;
    DynamicArray<UUID> m_containedUUIDs;

    QuadNode* m_parent = nullptr;
    std::array<std::unique_ptr<QuadNode>, 4> m_childNodes;

    void reset(void);
    bool is_in_node_boundary(const Extent& ext) const;
    void produce_children(const unsigned max_depth);

    QuadNode* insert_entity(const UUID& uuid, const Extent& ext);
    const QuadNode* find_node(const Extent& ext) const;

    bool has_parent(const QuadNode* node) const;
    void retrieve(DynamicArray<UUID>* candidates, const UUID collider) const;
    void retrieve_from_parents(DynamicArray<UUID>* candidatess, const UUID collider) const;
    void retrieve_from_children(DynamicArray<UUID>* candidatess, const UUID collider) const;

    QuadNode(QuadNode* parent, const unsigned depth_, float _x, float _y, float _w) :
        c_depth(depth_), c_x(_x), c_y(_y), c_w(_w), m_hasChildren(false), m_parent(parent) {
            m_containedUUIDs.reserve(64);
        }

	//TODO: destroy member arrays
};


class QuadTree {
private:
    const unsigned c_maxDepth;

public:
    QuadNode m_top;

    QuadNode* insert_entity(const UUID& uuid, const Extent& ext) { return m_top.insert_entity(uuid, ext); }
    const QuadNode* find_node(const Extent& ext) const { return m_top.find_node(ext); }
    void reset(void) { m_top.reset(); }

    QuadTree(void) = delete;

    QuadTree(unsigned max_depth);
};

