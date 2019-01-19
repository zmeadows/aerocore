#include "LocalVertexBuffer.hpp"


std::vector<v2> compute_global_vertices( const LocalVertexBuffer* lvb
                                       , const v2 position_offset
                                       , const float rotation_angle)
{
    std::vector<v2> gvb;
    gvb.reserve(lvb->count);

    for (u32 i = 0; i < lvb->count; i++) {
        gvb.push_back((*lvb)[i].rotated(rotation_angle) + position_offset);
    }

    return gvb;
}

