#include "../include/Automaton.h"

#include <queue>

using namespace std;

/**
 * @brief Converts a Non-Deterministic Finite Automaton (NFA) into
 *        an equivalent Deterministic Finite Automaton (DFA) using
 *        the **subset construction algorithm** (also known as powerset construction).
 *
 * @details
 * ### Theoretical Background:
 * Given an NFA `A = (Q_A, Σ, δ_A, I_A, F_A)`,
 * the subset construction builds a DFA `D = (Q_D, Σ, δ_D, I_D, F_D)` such that:
 *
 * - Each DFA state corresponds to a *set of NFA states*.
 * - The initial state of `D` is the set of all initial states of `A`.
 * - A DFA state `S ⊆ Q_A` is *final* if `S ∩ F_A ≠ ∅`.
 * - The transition function δ_D is defined as:
 *      δ_D(S, a) = ⋃_{q ∈ S} δ_A(q, a)
 *
 * This guarantees that the DFA recognizes exactly the same language
 * as the NFA — that is, `L(D) = L(A)`.
 *
 * @param A  The input automaton (can be NFA or ε-NFA without explicit ε-handling here).
 * @return   A deterministic automaton equivalent to `A`.
 *
 * @note
 * - This implementation does **not** handle ε-transitions (ε-NFA).
 *   Those should be eliminated first using ε-closure computation.
 * - A "dead" (sink) state is added if necessary to make the resulting DFA complete.
 */
Automaton Automaton::determinise(const Automaton& A) {
    Automaton D;                       // Resulting deterministic automaton
    map<set<int>, int> stateMapping;   // Maps subsets of NFA states → DFA state IDs
    queue<set<int>> q;                 // Work queue for unprocessed subsets

    int nextStateId = 0;               // Counter for assigning DFA state IDs

    // Step 1: Initialize the DFA start state.
    // ---------------------------------------
    // In subset construction, the initial DFA state is the set of all initial NFA states.
    set<int> start = A.initialStates;

    q.push(start);                     // Queue starts with the initial subset
    stateMapping[start] = nextStateId++;  // Assign DFA ID 0 to this subset

    D.initialStates.insert(0);         // DFA’s initial state is 0

    // Step 2: Process each subset (BFS traversal of subset space).
    // -------------------------------------------------------------
    while (!q.empty()) {
        set<int> current = q.front();
        q.pop();

        int currentId = stateMapping[current];
        D.states.insert(currentId);

        // Step 2a: Mark current DFA state as final if any NFA state in it is final.
        for (int state : current) {
            if (A.finalStates.count(state)) {
                D.finalStates.insert(currentId);
                break;
            }
        }

        // Step 2b: Compute transitions for every symbol in the alphabet.
        for (char c : A.alphabet) {
            set<int> nextSet;  // The new subset reached by reading symbol c

            // For each NFA state in the current subset, gather reachable states on 'c'.
            for (int state : current) {
                auto it = A.transitions.find({state, c});
                if (it != A.transitions.end()) {
                    nextSet.insert(it->second.begin(), it->second.end());
                }
            }

            // If no transitions exist for this symbol, skip it.
            if (nextSet.empty()) continue;

            // If this new subset of NFA states hasn’t been seen before,
            // assign it a new DFA state ID and enqueue it for processing.
            if (!stateMapping.count(nextSet)) {
                stateMapping[nextSet] = nextStateId++;
                q.push(nextSet);
            }

            // Create the DFA transition: currentId --c--> stateMapping[nextSet]
            D.transitions[{currentId, c}].insert(stateMapping[nextSet]);

            // Ensure the symbol is included in the DFA’s alphabet.
            D.alphabet.insert(c);
        }
    }

    // Step 3: Add a dead (sink) state to make the DFA total (complete).
    // -----------------------------------------------------------------
    // This ensures every state has a transition for every symbol.
    int deadState = nextStateId++;
    bool usedDead = false;

    for (int s : D.states) {
        for (char c : D.alphabet) {
            // If a transition is missing, send it to the dead state.
            if (!D.transitions.count({s, c})) {
                D.transitions[{s, c}].insert(deadState);
                usedDead = true;
            }
        }
    }

    // If the dead state was used, add self-loops on all symbols.
    if (usedDead) {
        D.states.insert(deadState);
        for (char c : D.alphabet) {
            D.transitions[{deadState, c}].insert(deadState);
        }
    }

    // Step 4: Return the constructed deterministic automaton.
    return D;
}
