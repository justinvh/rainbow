#include <sstream>
#include <utility>
#include <rainbow/file.hpp>

rb::Contents rb::read(const std::string& file,
                      std::ios_base::openmode mode)
{
    std::ifstream t(file.c_str(), mode);
    std::stringstream error;
    rb::Contents contents;

    if (!t.good()) {
        error << "failed to open file: " << file;
        contents.error = error.str();
        contents.valid = false;
    } else {
        t.seekg(0, std::ios::end);   
        contents.data.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        contents.valid = true;
        contents.data.assign((std::istreambuf_iterator<char>(t)),
                              std::istreambuf_iterator<char>());
    }

    t.close();

    return contents;
}
