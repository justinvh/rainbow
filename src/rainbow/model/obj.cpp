#include <iostream>
#include <fstream>
#include <sstream>

#include <rainbow/model/obj.hpp>
#include <rainbow/file.hpp>
#include <rainbow/math/random.hpp>
#include <rainbow/utils/logger.hpp>

using namespace rb;
using namespace std;

namespace {
    const char comment = '#';
    const char vertex = 'v';
    const char face = 'f';
}

bool handle_comment(Model_obj* instance, stringstream& iss)
{
    return false;
}

bool handle_vertex(Model_obj* instance, stringstream& iss)
{
    int p = instance->vert_position;
    float* v = instance->data_verts.get();
    iss >> v[p] >> v[p + 1] >> v[p + 2];

    // Store the color information
    v[p + 3] = random_scale();
    v[p + 4] = random_scale();
    v[p + 5] = random_scale();

    instance->vert_position += 6 + 3 + 2;
    return false;
}

bool handle_normal(Model_obj* instance, stringstream& iss)
{
    int p = instance->vert_position;
    float* n = instance->data_verts.get();
    p += 6;
    iss >> n[p] >> n[p + 1] >> n[p + 2];
    instance->vert_position += 3 + 2;
    return false;
}

bool handle_texture(Model_obj* instance, stringstream& iss)
{
    int p = instance->vert_position;
    float* n = instance->data_verts.get();
    p += 9;
    iss >> n[p] >> n[p + 1];
    instance->vert_position += 2;
    return false;
}

bool handle_face(Model_obj* instance, stringstream& iss)
{
    int* e = instance->data_elements.get();
    int p = instance->element_position;
    while ((iss.rdstate() & stringstream::eofbit) == 0) {
        iss >> e[p];
        e[p] -= 1;
        p += 1;
    }
    instance->element_position = p;
    return false;

}

Model_obj::Model_obj(const std::string& filename)
{
    valid = load_model(filename);
}

bool Model_obj::load_model(const std::string& filename)
{
    ifstream obj(filename.c_str());

    if (!obj.good()) {
        valid = false;
        return false;
    }

    string line;
    data_element_count = 0;
    data_element_size = 0;
    data_vert_count = 0;
    data_vert_size = 0;

    // Do a quick pass to count number of vertices and elements
    int faces = 0, vertices = 0;
    log_debug("Pass 0: Allocation");
    while((getline(obj, line, '\n').rdstate() & ifstream::eofbit) == 0) {
        string token;
        stringstream iss(line);
        iss >> token;
        char action = token[0];
        if (action == vertex) {
            // vertices, color, normal, texture
            data_vert_count += (3 + 3 + 3 + 2);
            vertices += 1;
        } else if (action == face) {
            faces++;
            while ((iss.rdstate() & stringstream::eofbit) == 0) {
                int temp;
                iss >> temp;
                data_element_count += 1;
            }
        }
    }

    data_verts = unique_ptr<float>(new float[data_vert_count]);
    data_elements = unique_ptr<int>(new int[data_element_count]);
    data_element_size = sizeof(int) * data_element_count;
    data_vert_size = sizeof(float) * data_vert_count;

    memset(data_verts.get(), 0.0, data_vert_size);
    memset(data_elements.get(), 0, data_element_size);

    // Allocate
    log_debug("data_vert_count: {}"_format(data_vert_count));
    log_debug("data_vert_size:  {}"_format(data_vert_size));
    log_debug("data_elements_count: {}"_format(data_element_count));
    log_debug("data_elements_size: "_format(data_element_size));
    log_debug("vertices: {}"_format(vertices));
    log_debug("faces: "_format(faces));

    // Reset
    obj.clear();
    obj.seekg(0, ios::beg);

    // Pass 1: Vertices and faces
    vert_position = 0;
    element_position = 0;
    log_debug("Pass 1: Vertices and faces");
    while((getline(obj, line, '\n').rdstate() & ifstream::eofbit) == 0) {
        string token;
        stringstream iss(line);
        bool continue_line = true;
        while (continue_line && (iss.rdstate() & stringstream::eofbit) == 0) {
            iss >> token;
            char action = token[0];
            switch (action) {
                case vertex:
                    continue_line = handle_vertex(this, iss);
                    break;
                case face:
                    continue_line = handle_face(this, iss);
                    break;
                default:
                    continue_line = handle_comment(this, iss);
                    break;
            }
        }
    }

    // Reset
    obj.clear();
    obj.seekg(0, ios::beg);

    // Pass 2 vn
    vert_position = 0;
    log_debug("Pass 2: Normals");
    while((getline(obj, line, '\n').rdstate() & ifstream::eofbit) == 0) {
        string token;
        stringstream iss(line);
        bool continue_line = true;
        while (continue_line && (iss.rdstate() & stringstream::eofbit) == 0) {
            iss >> token;
            if (token == "vn") {
                handle_normal(this, iss);
                break;
            }
        }
    }

    // Reset
    obj.clear();
    obj.seekg(0, ios::beg);

    // Pass 3 vt
    vert_position = 0;
    log_debug("Pass 3: Texture Units");
    while((getline(obj, line, '\n').rdstate() & ifstream::eofbit) == 0) {
        string token;
        stringstream iss(line);
        bool continue_line = true;
        while (continue_line && (iss.rdstate() & stringstream::eofbit) == 0) {
            iss >> token;
            if (token == "vt") {
                handle_texture(this, iss);
                break;
            }
        }
    }

    valid = true;
    return true;
}

// Satisfy the baseline models pure-virtual methods.
const float* Model_obj::verts() { return data_verts.get(); }
const float* Model_obj::uv() { return data_uv.get(); }
const int* Model_obj::elements() { return data_elements.get(); }
uint32_t Model_obj::vert_size() { return data_vert_size; }
uint32_t Model_obj::vert_count() { return data_vert_count; }
uint32_t Model_obj::element_size() { return data_element_size; }
uint32_t Model_obj::element_count() { return data_element_count; }
Model_format Model_obj::format() { return Model_format::OBJ; }
