#include <rainbow/model/obj.hpp>

using namespace rb;
using namespace std;

Model_obj::Model_obj(const std::string& filename)
{
    valid = load_model(filename);
}

bool Model_obj::load_model(const std::string& filename)
{
    return true;
}

// Satisfy the baseline models pure-virtual methods.
const float* Model_obj::verts() { return data_verts.get(); }
const float* Model_obj::uv() { return data_uv.get(); }
const int* Model_obj::elements() { return data_elements.get(); }
int Model_obj::vert_size() { return data_vert_size; }
int Model_obj::vert_count() { return data_vert_count; }
int Model_obj::element_size() { return data_element_size; }
int Model_obj::element_count() { return data_element_count; }
Model_format Model_obj::format() { return Model_format::OBJ; }
