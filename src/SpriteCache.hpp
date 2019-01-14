#pragma once

#include <unordered_map>

#include <SDL_gpu.h>

#include "Base.hpp"
#include "PolygonDecomposition.hpp"

struct SpriteCache {

    struct Entry {
        LocalVertexBuffer local_vertices;
        PolygonDecomposition poly_decomp;
        v2 image_offset;
        GPU_Image* image;
    };

    std::unordered_map<std::string, Entry> _store;

    void build_sprite(const std::string& key, const char* png_filepath, const char* xml_filepath);
    void attach_sprite_to_uuid(UUID uuid, const std::string& key);

    SpriteCache(const SpriteCache&) = delete;
    SpriteCache& operator=(const SpriteCache&) = delete;
    ~SpriteCache();
    SpriteCache();
};
