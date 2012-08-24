#include <string>
#include <sstream>
#include <iostream>

#include <rainbow/map/map.hpp>
#include <rainbow/file.hpp>

using namespace rb;
using namespace std;

Map::Map(const string& map_name)
{
    load_map(map_name);
}

bool Map::load_map(const string& map_name)
{
    Contents contents = read(map_name);
    if (!contents.valid)
        return false;

    // Tokenize our simple map
    stringstream tokens(contents.data);

    // Load the vertices
    int nv = 0;
    tokens >> nv;
    vert_count = 5 * nv;
    vert_size = vert_count * sizeof(float);
    verts = unique_ptr<float>(new float[vert_count]);
    for (int i = 0; i < nv; i++) {
        for (int j = 0; j < 5; j++) {
            tokens >> verts.get()[i * 5 + j];
        }
    }

    // Load the elements
    int ne = 0;
    tokens >> ne;
    element_count = ne;
    element_size = element_count * sizeof(int);
    elements = unique_ptr<int>(new int[element_count]);
    for (int i = 0; i < ne; i++) {
        tokens >> elements.get()[i];
    }
    
    valid = true;
    return true;
}
