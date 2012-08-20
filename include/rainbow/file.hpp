#ifndef RAINBOW_FILE_HPP
#define RAINBOW_FILE_HPP

#include <string>
#include <fstream>
#include <streambuf>
#include <tuple>

namespace rb {

struct Contents {
    std::string data;
    std::string error;
    bool valid;
};

Contents read(const std::string& file,
              std::ios_base::openmode mode = (std::ios_base::in |
                                              std::ios_base::binary));
}

#endif
