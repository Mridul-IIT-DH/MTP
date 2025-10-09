#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <map>
#include <set>
#include <string>
#include <vector>

class Automaton {
public:
    static Automaton readAutomaton(const std::string& filename);
    static Automaton determinise(const Automaton& A);
    static Automaton reverseTransitions(const Automaton& A);
    static Automaton minimize(const Automaton& A);
    static bool isIsomorphic(const Automaton& A, const Automaton& B, std::map<int, int>* mappingOut);

    void writeAutomaton(const std::string& filename) const;

    const std::set<int>& getStates() const { 
        return states; 
    }
    
    const std::set<int>& getInitialStates() const { 
        return initialStates; 
    }
    
    const std::set<int>& getFinalStates() const { 
        return finalStates; 
    }
    
    const std::set<char>& getAlphabet() const { 
        return alphabet; 
    }
    
    const std::map<std::pair<int, char>, std::set<int>>& getTransitions() const { 
        return transitions; 
    }

    void addState(int s) {
        states.insert(s);
    }

    void addInitialState(int s) {
        initialStates.insert(s);
    }

    void addFinalState(int s) {
        finalStates.insert(s);
    }

    void addTransition(int from, char symbol, int to) {
        transitions[{from, symbol}].insert(to);
        if (symbol != '#') alphabet.insert(symbol);
    }

    void setAlphabet(const std::set<char>& a) {
        alphabet = a;
    }

    void clearTransitions() {
        transitions.clear();
    }

private:
    std::set<int> states;
    std::set<int> initialStates;
    std::set<int> finalStates;
    std::set<char> alphabet;
    std::map<std::pair<int, char>, std::set<int>> transitions;
};

#endif