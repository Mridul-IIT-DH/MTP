#ifndef DOT_H
#define DOT_H

#include <string>

class Dot {
public:
    void generateDot(const std::string& inputFilename, const std::string& outputFilename, const std::string& graphName);
    void generateImage(const std::string& file, const std::string& prefix, const std::string& filename);
};

#endif