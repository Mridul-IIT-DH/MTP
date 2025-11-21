#include "../include/Automaton.h"

#include <map>
#include <queue>
#include <set>

using namespace std;

/**
 * Checks whether two deterministic finite automata (DFAs) A and B
 * are *isomorphic* — i.e., they are structurally identical except
 * for the naming (numbering) of their states.
 *
 * Formally, A and B are isomorphic if there exists a bijective mapping f
 * between their states such that:
 *  1. f(initial_A) = initial_B
 *  2. q ∈ Final_A ⇔ f(q) ∈ Final_B
 *  3. For every transition δ_A(q, a) = q', we have δ_B(f(q), a) = f(q')
 *
 * @param A The first automaton.
 * @param B The second automaton.
 * @param mappingOut (optional) If not null, will be filled with the state
 *                   mapping from A → B (i.e., f).
 * @return true if A and B are isomorphic, false otherwise.
 */
bool Automaton::isIsomorphic(const Automaton& A, const Automaton& B, map<int, int>* mappingOut) {

    // Step 1: Check that both automata use the same alphabet.
    // If alphabets differ, they cannot have identical transition structure.
    if (A.getAlphabet() != B.getAlphabet()) {
        return false;
    }

    // Step 2: For DFAs, there must be exactly one initial state.
    // If either automaton has more than one, this function is not applicable.
    if (A.getInitialStates().size() != 1 ||
        B.getInitialStates().size() != 1) {
        return false;
    }

    // Retrieve the single start states from both automata.
    int startA = *A.getInitialStates().begin();
    int startB = *B.getInitialStates().begin();

    // These maps store the bijective mapping between states:
    // mappingAB : A → B
    // mappingBA : B → A
    map<int, int> mappingAB;
    map<int, int> mappingBA;

    // We perform a BFS (Breadth-First Search) starting from the pair
    // (startA, startB). The BFS ensures we explore corresponding states
    // in both automata in a synchronized way.
    queue<pair<int, int>> q;
    q.push({startA, startB});

    // Initialize the mappings with the initial states.
    mappingAB[startA] = startB;
    mappingBA[startB] = startA;

    // BFS traversal loop
    while (!q.empty()) {
        auto [a, b] = q.front();
        q.pop();

        // Step 3: The "final" status of corresponding states must match.
        bool aFinal = A.getFinalStates().count(a);
        bool bFinal = B.getFinalStates().count(b);

        if (aFinal != bFinal) {
            // If one state is final and the other is not, not isomorphic.
            return false;
        }

        // Step 4: Check all transitions for every symbol in the alphabet.
        for (char c : A.getAlphabet()) {
            const auto& Atrans = A.getTransitions();
            const auto& Btrans = B.getTransitions();

            // Find transitions (a, c) → ?
            auto itA = Atrans.find({a, c});
            auto itB = Btrans.find({b, c});

            // Both automata must have a defined transition for this symbol.
            if (itA == Atrans.end() || itB == Btrans.end()) {
                return false;
            }

            // Since this is a DFA, each transition leads to exactly one state.
            int toA = *itA->second.begin();
            int toB = *itB->second.begin();

            // Step 5: If these target states have not yet been mapped,
            // create a new correspondence and continue BFS.
            if (!mappingAB.count(toA) && !mappingBA.count(toB)) {
                mappingAB[toA] = toB;
                mappingBA[toB] = toA;

                q.push({toA, toB});
            }
            // Step 6: If already mapped, check consistency:
            // the existing mapping must agree with this transition.
            else if (mappingAB.count(toA) && mappingAB[toA] != toB) {
                // Inconsistent mapping → not isomorphic.
                return false;
            }
        }
    }

    // Step 7: After BFS, every state in A must have a corresponding state in B,
    // and vice versa (bijective mapping).
    if (mappingAB.size() != A.getStates().size() ||
        mappingBA.size() != B.getStates().size()) {
        return false;
    }

    // Step 8: If user provided an output pointer, return the found mapping.
    if (mappingOut != nullptr) {
        *mappingOut = mappingAB;
    }

    // If we reach here, all checks passed — automata are isomorphic.
    return true;
}
