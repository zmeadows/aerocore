#include "QuadTreeDraw.hpp"

#include <SDL.h>
#include <SDL_gpu.h>

#include "unstd/types.hpp"
#include "GraphicsContext.hpp"
#include "QuadTree.hpp"
#include "Vector2D.hpp"

void drawQuadTree(GraphicsContext* GC, const QuadTree* quadTree) {
    drawQuadNodes(GC, &(quadTree->m_top), true);
    drawQuadNodes(GC, &(quadTree->m_top), false);
}

void drawQuadNodes(GraphicsContext* GC, const QuadNode* node, const bool draw_empty) {

    const u32 num_entities = node->m_containedUUIDs.size();

    if (
        (!draw_empty && num_entities > 0)
        ||
        (draw_empty && num_entities == 0)
       )
    {
        SDL_Color rgba;
        if (num_entities > 0) {
            rgba = { 255, 33, 33, 200 };
        } else {
            rgba = { 33, 255, 33, 100 };
        }

        const GameCoordinates p1 = { node->c_x, node->c_y };
        const GameCoordinates p2 = { node->c_x + node->c_w, node->c_y };
        const GameCoordinates p3 = { node->c_x + node->c_w, node->c_y - node->c_w};
        const GameCoordinates p4 = { node->c_x, node->c_y - node->c_w };

        // TODO: simplify using length-4 array
        ScreenCoordinates sc1 = GC->convert_to_screen_coords(p1);
        ScreenCoordinates sc2 = GC->convert_to_screen_coords(p2);
        ScreenCoordinates sc3 = GC->convert_to_screen_coords(p3);
        ScreenCoordinates sc4 = GC->convert_to_screen_coords(p4);

        GPU_Line(GC->renderer,
                 sc1.x, sc1.y,
                 sc2.x, sc2.y,
                 rgba);

        GPU_Line(GC->renderer,
                 sc2.x, sc2.y,
                 sc3.x, sc3.y,
                 rgba);

        GPU_Line(GC->renderer,
                 sc3.x, sc3.y,
                 sc4.x, sc4.y,
                 rgba);

        GPU_Line(GC->renderer,
                 sc4.x, sc4.y,
                 sc1.x, sc1.y,
                 rgba);
    }

    if (node->m_hasChildren) {
        for (auto& child_node : node->m_childNodes) {
            drawQuadNodes(GC, child_node.get(), draw_empty);
        }
    }
}
