#pragma once

#include <string>
#include <vector>

namespace basis {

using byte_t = char;

struct File {
    std::string filename;
    std::vector<byte_t> data;
};

}