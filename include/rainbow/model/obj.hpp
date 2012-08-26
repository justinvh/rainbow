#ifndef RAINBOW_MODEL_OBJ_HPP
#define RAINBOW_MODEL_OBJ_HPP

#include <memory>
#include <string>

#include <rainbow/model/model.hpp>

namespace rb {

class Model_obj : public Model {
public:
    ~Model_obj() {};
    Model_obj() = default;
    Model_obj(const std::string& filename);
    bool load_model(const std::string& filename);
public:
    virtual const float* verts();
    virtual const float* uv();
    virtual const int* elements();
    virtual uint32_t vert_size();
    virtual uint32_t vert_count();
    virtual uint32_t element_size();
    virtual uint32_t element_count();
    virtual Model_format format();
public:
    bool valid;
    int data_vert_size;
    int data_vert_count;
    int data_element_size;
    int data_element_count;
    int vert_position;
    int element_position;
    std::unique_ptr<float> data_verts;
    std::unique_ptr<float> data_uv;
    std::unique_ptr<int> data_elements;
};

}

#endif // RAINBOW_MODEL_OBJ_HPP
