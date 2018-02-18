#pragma once

#include "QuadTree.hpp"
#include "GraphicsContext.hpp"

void drawQuadTree(GraphicsContext* GC, const QuadTree* quadTree);
void drawQuadNodes(GraphicsContext* GC, const QuadNode* node, const bool draw_empty);
