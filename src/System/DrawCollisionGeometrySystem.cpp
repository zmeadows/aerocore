#include "System/DrawCollisionGeometrySystem.hpp"

#include "Component/CollisionData.hpp"
#include "Component/Common.hpp"
#include "Engine/ComponentManager.hpp"
#include "PolygonDecomposition.hpp"
#include "LocalVertexBuffer.hpp"
#include "GraphicsContext.hpp"

#include <SDL.h>

void DrawCollisionGeometrySystem::run(ComponentManager* CM, f32 dt) {
    SDL_Color polygon_color;
    polygon_color.r = 255;
    polygon_color.g = 0;
    polygon_color.b = 0;
    polygon_color.a = 255;

    for (const UUID uuid : this->followed) {
        const auto& entity = CM->get<Entity>(uuid);
        const auto& coldata = CM->get<CollisionData>(uuid);

        //TODO: don't reallocate a DynamicArray for each entity
        DynamicArray<v2> global_vertices = compute_global_vertices(coldata.local_vertices, entity.pos, entity.angle);
        Defer(global_vertices.deallocate());

        for (u32 pgon = 0; pgon < coldata.poly_decomp->count; pgon++) {
            PolygonRep pr = nth_polygon(coldata.poly_decomp, pgon);
            const size_t vtx_count = static_cast<size_t>(pr.count);

            for (size_t i = 0; i < vtx_count; i++) {
                ScreenCoordinates sc1 = GC->to_screen_coords(global_vertices[pr.indices[i]]);
                ScreenCoordinates sc2 = GC->to_screen_coords(global_vertices[pr.indices[(i+1) % vtx_count]]);

                GPU_Line(GC->renderer,
                         sc1.x, sc1.y,
                         sc2.x, sc2.y,
                         polygon_color);
            }
        }
    }
}

