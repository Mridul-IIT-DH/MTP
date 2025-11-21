#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <string>

using namespace std;

/**
 * @brief Implements **Proposition 3.13**:
 *
 *        *“The determinization of a co-deterministic, co-accessible automaton
 *        which recognizes a language L is the minimal automaton of L.”*
 *
 * @details
 * ---------------------------------------------------------------------------
 *  THEORETICAL CONTEXT — PROPOSITION 3.13
 * ---------------------------------------------------------------------------
 *
 * Let A be an automaton recognizing a language L.
 * Proposition 3.13 states that *if* A satisfies:
 *
 *   1. **Co-accessibility**:
 *        Every state can reach a final state.
 *        (All states are useful for accepting some continuation.)
 *
 *   2. **Co-determinism**:
 *        The reverse automaton Aᵗ (obtained by reversing all transitions
 *        and swapping initial ↔ final states) is deterministic.
 *
 *        Equivalently:
 *           “A is co-deterministic iff Aᵗ is deterministic.”
 *
 *   3. **Single final state restriction**:
 *        A must have exactly **one** final state for the proposition to apply.
 *
 * Then:
 *
 *      **Determinizing A yields the minimal DFA for L.**
 *
 * ---------------------------------------------------------------------------
 *  WHAT THIS FUNCTION DOES
 * ---------------------------------------------------------------------------
 * This function **assumes** the input automaton satisfies all conditions of
 * Proposition 3.13:
 *
 *    - The automaton is co-accessible.
 *    - The reversed automaton Aᵗ is deterministic (i.e., A is co-deterministic).
 *    - There is exactly one final state.
 *
 * Under these assumptions, *determinisation alone is sufficient* to produce the
 * minimal DFA for the language.
 *
 * Specifically:
 *    1. It takes the given NFA / co-deterministic automaton.
 *    2. Applies subset construction:   D = determinise(A).
 *    3. Writes D to "../../outputs/pro_<name>.txt".
 *    4. Generates DOT and PNG visualizations.
 *
 * This DFA D is mathematically guaranteed (by Proposition 3.13)
 * to be **the unique minimal DFA for L, up to isomorphism**.
 *
 * @param nonDeterministicAutomaton
 *        An automaton that *must* satisfy Prop. 3.13 assumptions.
 *
 * @param inputBaseName
 *        Base name used for output file generation.
 */
void proposition313(const Automaton& nonDeterministicAutomaton, const string& inputBaseName) {
    
    // Output file paths
    string outputTextFilePath = "../../outputs/pro_" + inputBaseName + ".txt";
    string outputDotFilePath  = "../../dots/pro_"  + inputBaseName + ".dot";

    // -----------------------------------------------------------------------
    // Step 1: Determinisation
    //
    // According to Proposition 3.13, since the automaton is:
    //    - co-accessible,
    //    - co-deterministic (Aᵗ deterministic),
    //    - has exactly one final state,
    // the result of determinisation is already the MINIMAL DFA.
    // -----------------------------------------------------------------------
    Automaton deterministicAutomaton = Automaton::determinise(nonDeterministicAutomaton);

    // -----------------------------------------------------------------------
    // Step 2: Write the determinized (and therefore minimal) automaton to file.
    // -----------------------------------------------------------------------
    deterministicAutomaton.writeAutomaton(outputTextFilePath);

    // -----------------------------------------------------------------------
    // Step 3: Visualization — DOT + PNG generation
    // -----------------------------------------------------------------------
    Dot dotGenerator;

    dotGenerator.generateDot(outputTextFilePath, outputDotFilePath, "Proposition_313_MinimalDFA");
    dotGenerator.generateImage(outputDotFilePath, "pro_", inputBaseName);
}
