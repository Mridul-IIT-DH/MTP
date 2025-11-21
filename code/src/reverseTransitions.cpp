#include "../include/Automaton.h"

/**
 * @brief Computes the **reverse (transpose) automaton** Aᵗ of a given automaton A.
 *
 * @details
 * The reverse automaton Aᵗ is defined as follows:
 *
 *   Let A = (Q, Σ, δ, I, F).
 *   Then the reversed automaton Aᵗ is:
 *
 *       Aᵗ = (Q, Σ, δᵗ, F, I)
 *
 *   That is:
 *     • The set of states Q remains unchanged.
 *     • The alphabet Σ is the same.
 *     • Initial and final states are swapped:
 *           Iᵗ = F
 *           Fᵗ = I
 *     • Every transition u --a--> v in A becomes:
 *           v --a--> u   in Aᵗ
 *
 * This reversal is central to several theoretical results, including:
 *   - Co-determinism: A is co-deterministic  ⇔  Aᵗ is deterministic.
 *   - Brzozowski’s DFA minimization method (reverse → determinise → reverse → determinise).
 *   - Language reversal: L(Aᵗ) = L(A)ʳ (the reverse of the language).
 *
 * @param A  The input automaton A whose transitions are to be reversed.
 *
 * @return The reversed automaton Aᵗ.
 */
Automaton Automaton::reverseTransitions(const Automaton& A) {
    Automaton R;

    // --------------------------------------------------------------
    // Step 1: Copy state set and alphabet directly (unchanged).
    // --------------------------------------------------------------
    R.states   = A.states;
    R.alphabet = A.alphabet;

    // --------------------------------------------------------------
    // Step 2: Swap initial and final states.
    //         This follows the formal definition of Aᵗ.
    // --------------------------------------------------------------
    R.initialStates = A.finalStates;
    R.finalStates   = A.initialStates;

    // --------------------------------------------------------------
    // Step 3: Reverse the direction of every transition.
    //
    //         For each transition (from --symbol--> to), add the
    //         reversed transition (to --symbol--> from).
    // --------------------------------------------------------------
    for (auto& [key, value] : A.transitions) {
        int from    = key.first;
        char symbol = key.second;

        for (int to : value) {
            R.transitions[{to, symbol}].insert(from);
        }
    }

    return R;
}
