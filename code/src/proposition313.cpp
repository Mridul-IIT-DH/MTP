#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <string>

using namespace std;

void proposition313(const Automaton& nonDeterministicAutomaton, const string& inputBaseName) {
    string outputTextFilePath = "../../outputs/pro_" + inputBaseName + ".txt";
    string outputDotFilePath = "../../dots/pro_" + inputBaseName + ".dot";

    Automaton deterministicAutomaton = Automaton::determinise(nonDeterministicAutomaton);

    deterministicAutomaton.writeAutomaton(outputTextFilePath);

    Dot dotGenerator;

    dotGenerator.generateDot(outputTextFilePath, outputDotFilePath, "Pro_DFA");


    dotGenerator.generateImage(outputDotFilePath, "pro_", inputBaseName);
}