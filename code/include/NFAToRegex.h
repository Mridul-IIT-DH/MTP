#ifndef NFA_TO_REGEX_H
#define NFA_TO_REGEX_H

#include "Automaton.h"
#include <string>

/**
 * @brief Converts an NFA (or DFA) into an equivalent regular expression
 *        using the **state elimination algorithm**.
 *
 * @details
 * The conversion follows the classical construction found in automata theory:
 *
 *      1. Convert automaton into a GNFA (Generalized NFA):
 *            - Add new unique start and final states.
 *            - Convert all transitions to regex labels.
 *
 *      2. Repeatedly eliminate states q ≠ start, final:
 *            R_ij ← R_ij  ∪ ( R_ik · (R_kk)* · R_kj )
 *
 *      3. When only start and final remain:
 *            The regex on the edge start → final is the result.
 *
 * The function supports:
 *   • NFAs
 *   • DFAs
 *   • ε-NFAs (ε-transitions represented as '#')
 *
 * The output regex may include:
 *   - Concatenation (implicit or explicit)
 *   - Union '|'
 *   - Kleene star '*'
 *   - Parentheses for grouping
 *   - '#' to denote ε (the empty string)
 *
 * Special cases:
 *   • If no string is accepted → returns the empty string "" (empty language).
 *   • If only ε is accepted → returns "#".
 *
 * @param A  The input automaton to convert.
 *
 * @return A regular expression equivalent to L(A).
 */
std::string automatonToRegex(const Automaton& A);

#endif
