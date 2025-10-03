#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <string>

using namespace std;

void brozozowskisAlgorithm(const Automaton& nonDeterministicAutomaton, const string& inputBaseName) {
    string outputFilePath = "../../outputs/bro_" + inputBaseName + ".txt";
    string outputDotFilePath = "../../dots/bro_" + inputBaseName + ".dot";

    Automaton At = Automaton::reverseTransitions(nonDeterministicAutomaton);
    Automaton det1 = Automaton::determinise(At);

    Automaton det1t = Automaton::reverseTransitions(det1);
    Automaton bro_DFA = Automaton::determinise(det1t);

    bro_DFA.writeAutomaton(outputFilePath);

    Dot dotGenerator;

    dotGenerator.generateDot(outputFilePath, outputDotFilePath, "bro_DFA");

    dotGenerator.generateImage(outputDotFilePath, "bro_", inputBaseName);
}