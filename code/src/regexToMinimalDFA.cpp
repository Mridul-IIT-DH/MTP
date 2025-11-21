#include "../include/RegexENFA.h"

#include <iostream>
#include <string>

using namespace std;

// Forward declaration (implemented elsewhere)
void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName);

/**
 * @brief Converts a regular expression into a **minimal DFA** using the full
 *        classical pipeline:
 *
 *          Regex → ε-NFA → NFA → DFA → Minimal DFA
 *
 * @details
 * This function performs the following stages:
 *
 *   -----------------------------------------------------------------------
 *   1. Read the regex from "../../inputs/<name>.txt".
 *
 *   2. Convert the regex into an ε-NFA using:
 *          regexToENFA()
 *      This uses **Thompson’s construction**, which always produces an
 *      equivalent ε-NFA.
 *
 *   3. Convert the ε-NFA → NFA using:
 *          eNFAtoNFA()
 *      This removes epsilon transitions by computing ε-closures.
 *
 *   4. Convert the NFA → minimal DFA using:
 *          minimalDFA()
 *      This applies:
 *         - Subset construction (determinisation)
 *         - DFA minimization
 *
 *   -----------------------------------------------------------------------
 *   Expected I/O Files Created:
 *     - ENFA written to outputs/enfa_<name>.txt
 *     - NFA  written to outputs/nfa_<name>.txt
 *     - Minimal DFA written to outputs/min_<name>.txt
 *     - DOT + image visualizations also generated for each stage.
 *
 * This function is primarily a **wrapper** providing a complete pipeline from
 * regex to minimal DFA with one user interaction step.
 *
 * @note
 * - The regex file must contain a *single-line* regular expression.
 * - The entire pipeline is deterministic: each stage produces a unique next
 *   automaton (up to isomorphism).
 */
void regexToMinimalDFA() {

    string regexBaseName;

    // -------------------------------------------------------------
    // Step 1: Ask user for base name of the regex input file.
    // -------------------------------------------------------------
    cout << "\nEnter the regex file name (without .txt): ";
    cin >> regexBaseName;

    // -------------------------------------------------------------
    // Step 2: Convert regex → εNFA using Thompson construction.
    // -------------------------------------------------------------
    Automaton enfa = regexToENFA(regexBaseName);

    // -------------------------------------------------------------
    // Step 3: Convert εNFA → NFA (remove epsilon transitions).
    // -------------------------------------------------------------
    Automaton nfa = eNFAtoNFA(regexBaseName);

    // -------------------------------------------------------------
    // Step 4: Convert NFA → minimal DFA.
    //         (Determinise + minimize internally.)
    // -------------------------------------------------------------
    minimalDFA(nfa, regexBaseName);
}
