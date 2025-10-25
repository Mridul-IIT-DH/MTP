#include "../include/Dot.h"

#include <string>

using namespace std;

void automatonToImage (const string& inputBaseName) {
    string inputTextFilePath = "../../inputs/" + inputBaseName + ".txt";
    string outputDotFilePath = "../../dots/automaton_" + inputBaseName + ".dot";

    Dot dotGenerator;

    dotGenerator.generateDot(inputTextFilePath, outputDotFilePath, "Automaton");
    dotGenerator.generateImage(outputDotFilePath, "automaton_", inputBaseName);

    return;
}