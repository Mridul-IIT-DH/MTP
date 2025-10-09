#include "../include/RegexENFA.h"

#include <iostream>
#include <string>

using namespace std;

void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void regexToMinimalDFA() {
            string regexBaseName;

            cout << "\nEnter the regex file name (without .txt): ";

            cin >> regexBaseName;

            Automaton enfa = regexToENFA(regexBaseName);
            Automaton nfa = eNFAtoNFA(regexBaseName);

            minimalDFA(nfa, regexBaseName);
}