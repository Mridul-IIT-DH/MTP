#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <string>

using namespace std;

/**
 * @brief Generates a minimal DFA from an input NFA (or DFA) and visualizes it.
 *
 * @details
 * This function performs the classical two-step DFA minimization pipeline:
 *
 *   1. **Determinisation** (Subset Construction)
 *         Converts the input NFA/DFA into a deterministic automaton.
 *         Ensures the automaton has exactly one transition per symbol from each state.
 *
 *   2. **Minimization**
 *         Applies a DFA minimization algorithm (typically partition refinement /
 *         Hopcroft or Moore algorithm depending on implementation).
 *         Removes redundant or equivalent states to produce a *minimal DFA*:
 *
 *                 The smallest DFA that recognizes the same language.
 *
 * After computing the minimal DFA:
 *   - It writes the resulting machine to a text file:
 *         ../../outputs/min_<name>.txt
 *   - Generates a DOT visualization:
 *         ../../dots/min_<name>.dot
 *   - Renders a PNG image:
 *         ../../images/min_<name>.png
 *
 * @param nonDeterministicAutomaton
 *        The input automaton (may be NFA or DFA).
 *
 * @param inputBaseName
 *        Base filename (no extension) used when constructing output filenames.
 */
void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName) {
    // Output paths for the minimal DFA
    string outputFilePath    = "../../outputs/min_" + inputBaseName + ".txt";
    string outputDotFilePath = "../../dots/min_"  + inputBaseName + ".dot";

    // ----------------------------------------------------------
    // Step 1: Determinise the input automaton.
    // ----------------------------------------------------------
    // Converts NFA → DFA.
    // If the input is already a DFA, determinise() simply ensures completeness.
    Automaton determinized = Automaton::determinise(nonDeterministicAutomaton);

    // ----------------------------------------------------------
    // Step 2: Minimize the determinized automaton.
    // ----------------------------------------------------------
    // Applies the chosen minimization algorithm implemented in Automaton::minimize().
    Automaton minimized = Automaton::minimize(determinized);

    // ----------------------------------------------------------
    // Step 3: Write minimized DFA to text file.
    // ----------------------------------------------------------
    minimized.writeAutomaton(outputFilePath);

    // ----------------------------------------------------------
    // Step 4: Generate DOT + PNG visualization.
    // ----------------------------------------------------------
    Dot dotGenerator;

    // Build DOT representation
    dotGenerator.generateDot(outputFilePath, outputDotFilePath, "minimized");

    // Render image using Graphviz
    dotGenerator.generateImage(outputDotFilePath, "min_", inputBaseName);
}
