#include "QuadTree.hpp"

#include <vector>
#include <assert.h>

void QuadNode::reset(void) {
    m_containedUUIDs.clear();

    if (m_hasChildren) {
        for (auto& node : m_childNodes)
            node->reset();
    }
}

QuadNode* QuadNode::insert_entity(const UUID& uuid, const Extent& ext) {
    if (!is_in_node_boundary(ext))
        return nullptr;

    if (m_hasChildren) {
        for (auto& node : m_childNodes) {
            QuadNode* inserted_node = node->insert_entity(uuid, ext);
            if (inserted_node)
                return inserted_node;
        }
    }

    m_containedUUIDs.push_back(uuid);
    return this;
}

void QuadNode::retrieve_from_parents(std::vector<UUID>& candidates, const UUID collider) {

    if (m_parent) {
        for (UUID uuid : m_parent->m_containedUUIDs) {
            if (uuid != collider)
                candidates.push_back(uuid);
        }
        m_parent->retrieve_from_parents(candidates, collider);
    }
}

void QuadNode::retrieve_from_children(std::vector<UUID>& candidates, const UUID collider) {

    for (UUID uuid : m_containedUUIDs) {
        if (uuid != collider)
            candidates.push_back(uuid);
    }

    if (m_hasChildren) {
        for (auto& node : m_childNodes) {
            node->retrieve_from_children(candidates, collider);
        }
    }
};

void QuadNode::retrieve(std::vector<UUID>& candidates, const UUID collider) {
    //@FIXME: sloppy
    candidates.clear();
    retrieve_from_parents(candidates, collider);
    retrieve_from_children(candidates, collider);
}

bool QuadNode::has_parent(const QuadNode* node) const {
    if (!node || (node->c_depth >= this->c_depth))
        return false;

    if (m_parent == node) {
        return true;
    } else if (m_parent != nullptr) {
        return m_parent->has_parent(node);
    }

    return false;
}

bool QuadNode::is_in_node_boundary(const Extent& ext) const {
    return ext.minX >= c_x
        && ext.maxX < c_x+c_w
        && ext.minY >= c_y - c_w
        && ext.maxY < c_y;
}

void QuadNode::produce_children(const unsigned max_depth) {
    assert(!m_hasChildren);

    if (c_depth == max_depth)
        return;

    const float new_w = c_w/2.f;

    m_childNodes[0].reset(new QuadNode(this, c_depth + 1 , c_x         , c_y         , new_w));
    m_childNodes[1].reset(new QuadNode(this, c_depth + 1 , c_x + new_w , c_y         , new_w));
    m_childNodes[2].reset(new QuadNode(this, c_depth + 1 , c_x         , c_y - new_w , new_w));
    m_childNodes[3].reset(new QuadNode(this, c_depth + 1 , c_x + new_w , c_y - new_w , new_w));

    m_hasChildren = true;

    m_childNodes[0]->produce_children(max_depth);
    m_childNodes[1]->produce_children(max_depth);
    m_childNodes[2]->produce_children(max_depth);
    m_childNodes[3]->produce_children(max_depth);
}

QuadTree::QuadTree(unsigned max_depth) :
    c_maxDepth(max_depth),
    m_top(QuadNode(nullptr, 0,-100.f, 100.f, 200.f))
{
    m_top.produce_children(c_maxDepth);
}

