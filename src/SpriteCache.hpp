#pragma once

#include "Vector2D.hpp"
#include "PolygonDecomposition.hpp"
#include "LocalVertexBuffer.hpp"

#include <SDL_gpu.h>

#include <unordered_map>

class UUID;
class ComponentManager;

struct SpriteCache {

    struct Entry {
        LocalVertexBuffer local_vertices;
        PolygonDecomposition poly_decomp;
        v2 image_offset;
        GPU_Image* image;
    };

    std::unordered_map<std::string, Entry> _store;

    void build_sprite(const std::string& key, const char* png_filepath, const char* xml_filepath);
    void attach_sprite_to_uuid(ComponentManager* CM, UUID uuid, const std::string& key);

    SpriteCache();
    ~SpriteCache();

    SpriteCache(const SpriteCache&)            = delete;
    SpriteCache& operator=(const SpriteCache&) = delete;
    SpriteCache(SpriteCache&&)                 = delete;
    SpriteCache& operator=(SpriteCache&&)      = delete;
};
