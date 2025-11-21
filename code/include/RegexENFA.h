#ifndef REGEX_ENFA_H
#define REGEX_ENFA_H

#include "Automaton.h"
#include <string>

/**
 * @brief Converts a regular expression into an equivalent ε-NFA (ENFA)
 *        using **Thompson’s construction**.
 *
 * @details
 * Thompson’s construction produces an ε-NFA with:
 *   - A unique start state and a unique final state.
 *   - ε-transitions (represented as '#') used heavily for:
 *        • Union (|)  
 *        • Concatenation  
 *        • Kleene star (*)  
 *
 * Construction rules used:
 *   - Symbol 'a' produces:   s --a--> t
 *   - Concatenation f1.f2:  connect f1.end --ε--> f2.start
 *   - Union f1|f2:          newStart →ε→ f1.start, newStart →ε→ f2.start  
 *                           f1.end →ε→ newEnd, f2.end →ε→ newEnd
 *   - Kleene star f*:       newStart →ε→ f.start, newStart →ε→ newEnd  
 *                           f.end →ε→ f.start, f.end →ε→ newEnd
 *
 * Input:
 *   The function reads the regular expression from:
 *       "../../inputs/<inputBaseName>.txt"
 *
 * Output:
 *   Returns an ε-NFA (in memory) and also writes/visualizes it:
 *       "../../outputs/enfa_<inputBaseName>.txt"
 *       "../../dots/enfa_<inputBaseName>.dot"
 *       "../../images/enfa_<inputBaseName>.png"
 *
 * @param inputBaseName Base filename (without extension) where the regex is stored.
 * @return The constructed ε-NFA.
 */
Automaton regexToENFA(const std::string& inputBaseName);

/**
 * @brief Converts an ε-NFA (ENFA) into a standard NFA
 *        by removing all ε-transitions.
 *
 * @details
 * Uses the classical **epsilon-closure construction**:
 *
 *   ε-closure(q) = set of all states reachable from q via ε-transitions.
 *
 * For each state-set S and each input symbol a:
 *     1. Compute target states reachable on 'a'.
 *     2. For each target state t, add ε-closure(t).
 *
 * Initial states:
 *     ε-closure of the original initial states.
 *
 * Final states:
 *     Any state whose ε-closure contains a final state of the ENFA.
 *
 * Input:
 *   Reads the ENFA produced by regexToENFA:
 *       "../../outputs/enfa_<inputBaseName>.txt"
 *
 * Output:
 *   Returns the equivalent NFA (in memory) and produces:
 *       "../../outputs/nfa_<inputBaseName>.txt"
 *       "../../dots/nfa_<inputBaseName>.dot"
 *       "../../images/nfa_<inputBaseName>.png"
 *
 * @param inputBaseName Base filename (identifies which ENFA to read).
 * @return The constructed NFA.
 */
Automaton eNFAtoNFA(const std::string& inputBaseName);

#endif
