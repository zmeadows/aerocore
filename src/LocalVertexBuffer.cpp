#include "LocalVertexBuffer.hpp"

DynamicArray<v2> compute_global_vertices( const LocalVertexBuffer* lvb
                                       , const v2 position_offset
                                       , const float rotation_angle)
{
    DynamicArray<v2> gvb;
    reserve_memory(&gvb, lvb->count);

    for (u32 i = 0; i < lvb->count; i++) {
        append(&gvb, (*lvb)[i].rotated(rotation_angle) + position_offset);
    }

    return gvb;
}

