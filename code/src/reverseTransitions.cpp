#include "../include/Automaton.h"

Automaton Automaton::reverseTransitions(const Automaton& A) {
    Automaton R;

    R.states = A.states;
    R.initialStates = A.finalStates;
    R.finalStates = A.initialStates;
    R.alphabet = A.alphabet;
    
    for (auto& [key, value] : A.transitions) {
        int from = key.first;
        char symbol = key.second;

        for (int to : value) {
            R.transitions[{to, symbol}].insert(from);
        }
    }

    return R;
}