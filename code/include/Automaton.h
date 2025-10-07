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

private:
    std::set<int> states;
    std::set<int> initialStates;
    std::set<int> finalStates;
    std::set<char> alphabet;
    std::map<std::pair<int, char>, std::set<int>> transitions;
};

#endif