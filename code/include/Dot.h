#ifndef DOT_H
#define DOT_H

#include "Automaton.h"

#include <map>
#include <string>

class Dot {
public:
    void generateDot(const std::string& inputFilename, const std::string& outputFilename, const std::string& graphName);
    void generateImage(const std::string& file, const std::string& prefix, const std::string& filename);
    void generateIsomorphismDot(
    const Automaton& A,
    const Automaton& B,
    const std::map<int, int>& mapping,
    const std::string& filenameBase
    );
};

#endif