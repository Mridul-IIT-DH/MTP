#include "../include/Automaton.h"

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <vector>

using namespace std;

/**
 * @brief Minimizes a deterministic finite automaton (DFA) using
 *        **partition refinement** (a classical DFA minimization algorithm).
 *
 * @details
 * ### Theoretical Background
 *
 * Minimization constructs the **smallest DFA** (fewest states) that recognizes
 * the same language. Two states in a DFA are **equivalent** if, for every input
 * string, they both either accept or both reject.
 *
 * Partition refinement algorithm:
 *   1. Partition states into two sets:
 *        - Final states  (accepting)
 *        - Non-final states (rejecting)
 *
 *   2. Repeatedly split partitions:
 *      Two states belong in the same partition *only if* their transitions
 *      under every symbol lead into the same partitions.
 *
 *   3. When no more splits occur, each partition represents **one state**
 *      in the minimized DFA, and all states in the same partition are
 *      formally equivalent.
 *
 * This algorithm is essentially Moore’s minimization algorithm.
 *
 * @param A  Deterministic automaton to be minimized.
 * @return   A new Automaton representing the minimal DFA equivalent to A.
 */
Automaton Automaton::minimize(const Automaton& A) {

    // ------------------------------------------------------------
    // Step 1: Initial Partition — split into final / non-final.
    // ------------------------------------------------------------
    set<int> nonFinalStates;

    for (int s : A.states) {
        if (!A.finalStates.count(s)) {
            nonFinalStates.insert(s);
        }
    }

    // partitions[i] = a block of states that are currently considered equivalent
    vector<set<int>> partitions;

    if (!nonFinalStates.empty())
        partitions.push_back(nonFinalStates);

    if (!A.finalStates.empty())
        partitions.push_back(A.finalStates);

    // ------------------------------------------------------------
    // Step 2: Refinement loop
    // Keep splitting partitions until no changes occur.
    // ------------------------------------------------------------
    bool changed = true;

    while (changed) {
        changed = false;
        vector<set<int>> newPartitions;

        // Process each partition separately
        for (auto& part : partitions) {
            
            // A map: signature → subset of states having that signature.
            // Signature identifies transition behavior of each state.
            map<vector<int>, set<int>> splitter;

            // Try splitting based on transitions under each alphabet symbol.
            for (int state : part) {
                vector<int> signature;

                // Build the signature: for each symbol, record which partition
                // the transition target belongs to.
                for (char c : A.alphabet) {
                    int targetPartitionIndex = -1;

                    auto it = A.transitions.find({state, c});
                    if (it != A.transitions.end()) {

                        // Since this is a DFA, each transition has exactly one target.
                        int to = *it->second.begin();

                        // Find which partition the target state belongs to
                        for (size_t i = 0; i < partitions.size(); i++) {
                            if (partitions[i].count(to)) {
                                targetPartitionIndex = (int)i;
                                break;
                            }
                        }
                    }

                    // Push the index of the partition reached by symbol c
                    signature.push_back(targetPartitionIndex);
                }

                // Group states by signature
                splitter[signature].insert(state);
            }

            // If no split occurred (only one signature), push partition as-is.
            if (splitter.size() == 1) {
                newPartitions.push_back(part);
            } 
            else {
                // A split occurred: different states have different signatures.
                // Add each “piece” as a new partition.
                for (auto& kv : splitter) {
                    newPartitions.push_back(kv.second);
                }
                changed = true;
            }
        }

        // Update partitions after refinement iteration
        partitions = newPartitions;
    }

    // ------------------------------------------------------------
    // Step 3: Build the minimized DFA.
    // Each partition becomes one state in the new DFA.
    // ------------------------------------------------------------
    Automaton M;
    map<int, int> stateToPartition;

    // Assign each partition an integer ID (0,1,2,...)
    for (size_t i = 0; i < partitions.size(); i++) {
        for (int s : partitions[i]) {
            stateToPartition[s] = (int)i;
        }
        M.states.insert((int)i);
    }

    // Initial states: map each original initial state to its partition
    for (int s : A.initialStates) {
        M.initialStates.insert(stateToPartition[s]);
    }

    // Final states: if any original final state sits in partition P,
    // then P is final
    for (int s : A.finalStates) {
        M.finalStates.insert(stateToPartition[s]);
    }

    // ------------------------------------------------------------
    // Step 4: Build transitions of minimized DFA
    // ------------------------------------------------------------
    for (auto& p : A.transitions) {
        int from = p.first.first;
        char symbol = p.first.second;
        int to = *p.second.begin();   // DFA: single transition target

        int newFrom = stateToPartition[from];
        int newTo   = stateToPartition[to];

        M.transitions[{newFrom, symbol}].insert(newTo);
        M.alphabet.insert(symbol);
    }

    return M;
}
