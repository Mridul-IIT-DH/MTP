#include "../include/Dot.h"

#include <string>

void Dot::generateImage(const std::string& file, const std::string& prefix, const std::string& filename) {
    std::string command = "mkdir -p ../../images && dot -Tpng " + file + " -o ../../images/" + prefix + filename + ".png";

    system(command.c_str());
}