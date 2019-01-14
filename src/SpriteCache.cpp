#include "SpriteCache.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "tinyxml2/tinyxml2.h"

#include "Entity.hpp"
#include "Globals.hpp"
#include "Geometry.hpp"

namespace {

struct SVGParsePathResult {
    LocalVertexBuffer vertices;
    v2 sprite_offset;
};

std::string trim(const std::string& str, const std::string& whitespace = " \t") {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string
reduce(const std::string& str, const std::string& fill = " ", const std::string& whitespace = " \t") {
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos) {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

std::vector<std::string> split_string(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos) {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

SVGParsePathResult parse_svg_path(const char* filepath) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filepath);
    tinyxml2::XMLElement* svg_element = doc.FirstChildElement("svg");
    tinyxml2::XMLElement* path_element = svg_element->FirstChildElement("path");

    const std::string path = std::string(path_element->Attribute("d"));
    const char* dims = svg_element->Attribute("viewBox");

    int minx, miny, width, height;

    sscanf(dims, "%d %d %d %d", &minx, &miny, &width, &height);

    SVGParsePathResult result;

    std::vector<std::string> cleaned_lines;
    for (std::string& line : split_string(path, "\n")) {
        std::string frmt_str = reduce(trim(line));
        if (frmt_str.front() == 'M')
            continue;
        else if (frmt_str.front() == 'C')
            frmt_str.erase(0, 2);

        v2 next_vtx;

        frmt_str.erase(frmt_str.find_first_of(' '), frmt_str.size());
        if (sscanf(frmt_str.c_str(), "%f,%f", &next_vtx.x, &next_vtx.y) != 2) {
            std::cout << "goofed." << std::endl;
        } else {
            result.vertices[result.vertices.count] = next_vtx;
            result.vertices.count++;
        }
    }

    for (v2& vtx : result.vertices) {
        vtx.x = vtx.x - static_cast<float>(width) / 2.f;
        vtx.y = vtx.y - static_cast<float>(height) / 2.f;
    }


    for (v2& vtx : result.vertices) {
        vtx.x = 200.f * vtx.x / 800.f;
        vtx.y = 200.f * vtx.y / 800.f;
    }

    return result;
}

} // end anonymous namespace

void SpriteCache::build_sprite(const std::string& key, const char* png_file, const char* xml_filepath) {
    assert(_store.find(key) == _store.end());

    SpriteCache::Entry entry;

    entry.image = GPU_LoadImage(asset_path(png_file).c_str());

    SVGParsePathResult parse_result = parse_svg_path(asset_path(xml_filepath).c_str());

    entry.local_vertices = parse_result.vertices;
    entry.image_offset = parse_result.sprite_offset;
    entry.poly_decomp = decompose_local_vertices(&entry.local_vertices);

    _store[key] = entry;
}

void SpriteCache::attach_sprite_to_uuid(UUID uuid, const std::string& key) {
    auto CM = get_manager();

    assert(_store.find(key) != _store.end());
    const SpriteCache::Entry& entry = _store[key];

    auto& sprite = CM->book<Sprite>(uuid);
    sprite.image = entry.image;
    sprite.offset = entry.image_offset;

    CM->book<CollisionData>(uuid, &entry.local_vertices, &entry.poly_decomp);

    auto& entity = CM->get<Entity>(uuid);
    entity.extent = extent_of(&entry.local_vertices, entity.pos, entity.angle);

    CM->book<PositionUpdate>(uuid);
    CM->book<RotationUpdate>(uuid);
}

SpriteCache::~SpriteCache() {
    for (auto kv : _store) {
        GPU_FreeImage(kv.second.image);
    }
}

SpriteCache::SpriteCache() {
    build_sprite("twister_idle", "sprites/twister/twister.png", "sprites/twister/path");
    build_sprite("bullet_player", "sprites/twister/twister.png", "sprites/twister/path");
    build_sprite("bullet_enemy", "sprites/twister/twister.png", "sprites/twister/path");
    build_sprite("player_idle", "sprites/player/player.png", "sprites/player/path");
}
