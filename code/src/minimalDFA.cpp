#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <string>

using namespace std;

void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName) {
    string outputFilePath = "../../outputs/min_" + inputBaseName + ".txt";
    string outputDotFilePath = "../../dots/min_" + inputBaseName + ".dot";

    Automaton determinized = Automaton::determinise(nonDeterministicAutomaton);
    Automaton minimized = Automaton::minimize(determinized);

    minimized.writeAutomaton(outputFilePath);

    Dot dotGenerator;

    dotGenerator.generateDot(outputFilePath, outputDotFilePath, "minimized");

    dotGenerator.generateImage(outputDotFilePath, "min_", inputBaseName);
}