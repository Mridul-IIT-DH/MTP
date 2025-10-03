#include "../include/Automaton.h"

#include <queue>

using namespace std;

Automaton Automaton::determinise(const Automaton& A) {
    Automaton D;
    map<set<int>, int> stateMapping;
    queue<set<int>> q;

    int nextStateId = 0;

    set<int> start = A.initialStates;

    q.push(start);

    stateMapping[start] = nextStateId++;
    
    D.initialStates.insert(0);

    while (!q.empty()) {
        set<int> current = q.front(); 
        q.pop();

        int currentId = stateMapping[current];
        D.states.insert(currentId);

        for (int state : current) {
            if (A.finalStates.count(state)) { 
                D.finalStates.insert(currentId);
                
                break;
            }
        }

        for (char c : A.alphabet) {
            set<int> nextSet;

            for (int state : current) {
                auto it = A.transitions.find({state, c});

                if (it != A.transitions.end())
                    nextSet.insert(it->second.begin(), it->second.end());
            }

            if (nextSet.empty()) {
                continue;
            }

            if (!stateMapping.count(nextSet)) {
                stateMapping[nextSet] = nextStateId++;

                q.push(nextSet);
            }

            D.transitions[{currentId, c}].insert(stateMapping[nextSet]);

            D.alphabet.insert(c);
        }
    }

    int deadState = nextStateId++;
    bool usedDead = false;

    for (int s : D.states) {
        for (char c : D.alphabet) {
            if (!D.transitions.count({s, c})) {
                D.transitions[{s, c}].insert(deadState);
                usedDead = true;
            }
        }
    }

    if (usedDead) {
        D.states.insert(deadState);
        for (char c : D.alphabet) {
            D.transitions[{deadState, c}].insert(deadState);
        }
    }


    return D;
}