#pragma once

#include <array>
#include <memory>
#include <set>

#include "Base.hpp"
#include "UUID.hpp"

class QuadTree {
private:
    struct Node {
        const unsigned depth;
        const float x, y, w;

        Node* parent;

        std::array<std::unique_ptr<Node>, 4> children;

        std::set<UUID> containedUUIDs;

        bool inBounds(const Position& pos) const;
        bool hasChildren(void) const { return children[0] != nullptr; }

        Node(const unsigned depth_) : depth(depth_) {}
    };

    Node m_top;
    const float m_minimumNodeWidth;

    Node* insert(const UUID& uuid, const Position& pos);
    Node* update(const UUID& uuid, Node* currentNode, const Position& newPos);

    // std::vector<UUID> collisionCandidates (across all nodes)
public:
};

bool QuadTree::Node::inBounds(const Position& pos) const { return false; }
