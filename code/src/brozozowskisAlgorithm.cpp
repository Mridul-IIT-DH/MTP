#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <string>

using namespace std;

/**
 * @brief Minimizes a deterministic finite automaton (DFA)
 *        using **Brzozowski’s algorithm**.
 *
 * Brzozowski’s algorithm is a conceptually simple but powerful
 * DFA minimization method based on **two reversals and two
 * determinisations**.
 *
 * The theoretical procedure is:
 *    1. Reverse the automaton (swap start and final states,
 *       and reverse transition directions).
 *    2. Determinise the reversed automaton (subset construction).
 *    3. Reverse the result again.
 *    4. Determinise again.
 *
 * The resulting automaton is guaranteed to be the **minimal DFA**
 * equivalent to the original automaton.
 *
 * Unlike other algorithms (like Hopcroft’s or Moore’s), Brzozowski’s
 * algorithm works by converting nondeterministic automata to deterministic
 * ones repeatedly, exploiting the duality between determinisation
 * and reversal. Although less efficient, it is very elegant and
 * easy to implement.
 *
 * @param nonDeterministicAutomaton
 *        The input automaton (can be an NFA or DFA).
 * @param inputBaseName
 *        Base name used to construct output file and image names.
 *        For example, if "dfa1" is given, outputs will be:
 *        - "../../outputs/bro_dfa1.txt"
 *        - "../../dots/bro_dfa1.dot"
 *
 * Output:
 *   - Writes the minimal DFA to a text file in `outputs/`.
 *   - Generates a corresponding `.dot` file and image visualization
 *     for the minimized automaton using the `Dot` utility class.
 */
void brozozowskisAlgorithm(const Automaton& nonDeterministicAutomaton, const string& inputBaseName) {
    // Step 0: Construct output file paths
    // -----------------------------------
    // Output text file will contain the minimized DFA description.
    string outputFilePath = "../../outputs/bro_" + inputBaseName + ".txt";

    // Output DOT file will store the Graphviz representation of the minimized automaton.
    string outputDotFilePath = "../../dots/bro_" + inputBaseName + ".dot";

    // Step 1: Reverse the transitions of the input automaton.
    // -------------------------------------------------------
    // This swaps initial and final states and reverses all transition directions.
    // The resulting automaton may be nondeterministic.
    Automaton At = Automaton::reverseTransitions(nonDeterministicAutomaton);

    // Step 2: Determinise the reversed automaton.
    // -------------------------------------------
    // The subset construction converts the reversed NFA into an equivalent DFA.
    Automaton det1 = Automaton::determinise(At);

    // Step 3: Reverse the transitions again.
    // --------------------------------------
    // Now we reverse the first determinised automaton.
    Automaton det1t = Automaton::reverseTransitions(det1);

    // Step 4: Determinise once more.
    // -------------------------------
    // The final determinisation step ensures the resulting automaton is deterministic
    // and minimal — this is the minimal DFA equivalent to the original automaton.
    Automaton bro_DFA = Automaton::determinise(det1t);

    // Step 5: Write the resulting minimal DFA to a text file.
    bro_DFA.writeAutomaton(outputFilePath);

    // Step 6: Generate visualization for the minimized automaton.
    // -----------------------------------------------------------
    // Create an instance of the Dot class for generating DOT and image files.
    Dot dotGenerator;

    // Generate the DOT file representing the minimized automaton.
    dotGenerator.generateDot(outputFilePath, outputDotFilePath, "bro_DFA");

    // Generate a visual image (e.g., PNG/SVG) from the DOT file.
    dotGenerator.generateImage(outputDotFilePath, "bro_", inputBaseName);
}
