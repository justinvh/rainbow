#ifndef RAINBOW_MAP_MAP_HPP
#define RAINBOW_MAP_MAP_HPP

#include <memory>

namespace rb {

class Map {
public:
    Map() = default;
    Map(const std::string& map_name);
    bool load_map(const std::string& map_name);

public:
    std::unique_ptr<float> verts;
    std::unique_ptr<int> elements;
    uint32_t vert_count;
    uint32_t vert_size;
    uint32_t element_count;
    uint32_t element_size;
    bool valid;
};

}

#endif // RAINBOW_MAP_MAP_HPP
