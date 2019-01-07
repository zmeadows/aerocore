#include "Entity.hpp"
#include "Geometry.hpp"
#include "GraphicsContext.hpp"
#include "Sprite.hpp"

#include <vector>
#include <string>
#include <iostream>

#include "tinyxml2/tinyxml2.h"

namespace {

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string reduce(const std::string& str,
                   const std::string& fill = " ",
                   const std::string& whitespace = " \t")
{
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

}

// void assign_iso_triangle_vertices(Entity& entity, float base_width, float height)
// {
//     entity.local_vertices.count = 3;
//     entity.local_vertices[0] = {0.f, base_width / 2.f};
//     entity.local_vertices[1] = {height, 0.f};
//     entity.local_vertices[2] = {0.f, -base_width / 2.f};
// 
//     recenter(entity.local_vertices);
// }

void draw(GraphicsContext& GC, const Sprite& sprite, v2 pos, float angle)
{

    // v2 orientation = orientation_of(entity);
    // orientation.scale(10.f);
    // v2 orientation_dot = entity.pos + orientation;

    // GPU_CircleFilled(GC->renderer,
    //                  GC->to_screen_coords(orientation_dot).x,
    //                  GC->to_screen_coords(orientation_dot).y,
    //                  4,
    //                  { 30,225,30,175});

    // GPU_Line(GC->renderer,
    //                  GC->to_screen_coords(entity.pos).x,
    //                  GC->to_screen_coords(entity.pos).y,
    //                  GC->to_screen_coords(orientation_dot).x,
    //                  GC->to_screen_coords(orientation_dot).y,
    //                  { 30,225,30,175});

    // GPU_CircleFilled(GC->renderer,
    //                  GC->to_screen_coords(global_vertices[0]).x,
    //                  GC->to_screen_coords(global_vertices[0]).y,
    //                  10,
    //                  { 0,0,255,255});

    // const size_t vtx_count = entity.global_vertices.count;

    if (sprite.image != nullptr) {
        ScreenCoordinates sc = GC.to_screen_coords(pos);
        const v2 offset = sprite.offset.rotated(-angle);
        GPU_BlitRotate(sprite.image, NULL, GC.renderer,
                       sc.x - offset.x, sc.y - offset.y,
                       - angle * 180.f/PI);
    }
    // else {
    //     for (size_t i = 0; i < vtx_count; i++) {
    //         ScreenCoordinates sc1 = GC.to_screen_coords(entity.global_vertices[i]);
    //         ScreenCoordinates sc2 = GC.to_screen_coords(entity.global_vertices[(i+1) % vtx_count]);
    //         GPU_Line(GC.renderer,
    //                  sc1.x, sc1.y,
    //                  sc2.x, sc2.y,
    //                  entity.color);
    //     }
    // }
}


SVGParsePathResult parse_svg_path(const char* filepath)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filepath);
    tinyxml2::XMLElement* svg_element = doc.FirstChildElement( "svg" );
    tinyxml2::XMLElement* path_element = svg_element->FirstChildElement( "path" );

    const std::string path = std::string(path_element->Attribute( "d" ));
    const char* dims = svg_element->Attribute( "viewBox" );

    int minx, miny, width, height;

    sscanf(dims, "%d %d %d %d", &minx, &miny, &width, &height);

    SVGParsePathResult result;

    std::vector<std::string> cleaned_lines;
    for (std::string& line : split_string(path, "\n")) {
        std::string frmt_str = reduce(trim(line));
        if (frmt_str.front() == 'M')
            continue;
        else if (frmt_str.front() == 'C')
            frmt_str.erase(0,2);

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
        vtx.x = vtx.x - static_cast<float>(width)/2.f;
        vtx.y = vtx.y - static_cast<float>(height)/2.f;
    }

    result.sprite_offset = recenter(result.vertices);

    for (v2& vtx : result.vertices) {
        vtx.x = 200.f * vtx.x / 800.f;
        vtx.y = 200.f * vtx.y / 800.f;
    }

    return result;
}
