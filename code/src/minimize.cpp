#include "../include/Automaton.h"

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <vector>

using namespace std;

Automaton Automaton::minimize(const Automaton& A) {
    set<int> nonFinalStates;

    for (int s : A.states) {
        if (!A.finalStates.count(s)) {
            nonFinalStates.insert(s);
        }
    }

    vector<set<int>> partitions;

    if (!nonFinalStates.empty()) {
        partitions.push_back(nonFinalStates);
    }

    if (!A.finalStates.empty()) {
        partitions.push_back(A.finalStates);
    }

    bool changed = true;

    while (changed) {
        changed = false;

        vector<set<int>> newPartitions;

        for (auto &part : partitions) {
            map<vector<int>, set<int>> splitter;

            for (int state : part) {
                vector<int> signature;

                for (char c : A.alphabet) {
                    int target = -1;

                    auto it = A.transitions.find({state, c});

                    if (it != A.transitions.end()) {
                        int to = *it->second.begin();

                        for (size_t i = 0; i < partitions.size(); i++) {
                            if (partitions[i].count(to)) {
                                target = (int)i;

                                break;
                            }
                        }
                    }

                    signature.push_back(target);
                }

                splitter[signature].insert(state);
            }

            if (splitter.size() == 1) {
                newPartitions.push_back(part);
            } else {
                for (auto &kv : splitter) {
                    newPartitions.push_back(kv.second);
                }

                changed = true;
            }
        }

        partitions = newPartitions;
    }

    Automaton M;
    map<int, int> stateToPartition;

    for (size_t i = 0; i < partitions.size(); i++) {
        for (int s : partitions[i]) {
            stateToPartition[s] = (int)i;
        }
        
        M.states.insert((int)i);
    }

    for (int s : A.initialStates) {
        M.initialStates.insert(stateToPartition[s]);
    }

    for (int s : A.finalStates) {
        M.finalStates.insert(stateToPartition[s]);
    }

    for (auto &p : A.transitions) {
        int from = p.first.first;
        char symbol = p.first.second;
        int to = *p.second.begin(); 

        M.transitions[{stateToPartition[from], symbol}].insert(stateToPartition[to]);

        M.alphabet.insert(symbol);
    }

    return M;
}