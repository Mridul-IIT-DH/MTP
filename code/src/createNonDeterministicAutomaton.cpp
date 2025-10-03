#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <string>

using namespace std;

Automaton createNonDeterministicAutomaton(const string& inputBaseName) {
    string inputTextFilePath = "../../inputs/" + inputBaseName + ".txt";
    string outputDotFilePath = "../../dots/nfa_" + inputBaseName + ".dot";

    Automaton nonDeterministicAutomaton = Automaton::readAutomaton(inputTextFilePath);

    Dot dotGenerator;

    dotGenerator.generateDot(inputTextFilePath, outputDotFilePath, "NFA");
    dotGenerator.generateImage(outputDotFilePath, "nfa_", inputBaseName);

    return nonDeterministicAutomaton;
}