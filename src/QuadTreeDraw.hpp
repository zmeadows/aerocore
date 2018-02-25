#pragma once

class GraphicsContext;
class QuadTree;
struct QuadNode;

void drawQuadTree(GraphicsContext* GC, const QuadTree* quadTree);
void drawQuadNodes(GraphicsContext* GC, const QuadNode* node, const bool draw_empty);
