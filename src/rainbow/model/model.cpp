#include <string>
#include <functional>
#include <map>
#include <cassert>

#include <rainbow/model.hpp>

using namespace rb;
using namespace std;

typedef function<Model_unique(const std::string&)> Model_function;

template<class T>
Model_function wrap()
{
    return [](const std::string& m) {return Model_unique(new T(m));};
}

Model_format rb::guess_model_format(const std::string& filename)
{
    static map<string, Model_format> supported_models = {
        {"obj", Model_format::OBJ}
    };

    // Verify that we can even try to load the model
    int idx = filename.find_last_of(".");
    if (idx == std::string::npos)
        return Model_format::UNKNOWN;

    // Attempt to 
    std::string ext = filename.substr(idx + 1);
    auto model = supported_models.find(ext);
    if (model == supported_models.end())
        return Model_format::UNKNOWN;

    return model->second;
}

Model_unique rb::load_model(const std::string& filename, Model_format fmt)
{
    static map<Model_format, Model_function> supported_models = {
        { Model_format::OBJ, wrap<Model_obj>() },
    };

    if (fmt != Model_format::UNKNOWN) 
        return supported_models[fmt](filename);

    Model_format guessed_format = guess_model_format(filename);
    assert(guessed_format != Model_format::UNKNOWN &&
           "Model is in a format that is unavailable to the engine.");
    return supported_models[guessed_format](filename);
}
