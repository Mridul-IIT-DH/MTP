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

    void writeAutomaton(const std::string& filename) const;

private:
    std::set<int> states;
    std::set<int> initialStates;
    std::set<int> finalStates;
    std::set<char> alphabet;
    std::map<std::pair<int, char>, std::set<int>> transitions;
};

#endif