#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <string>

using namespace std;

/**
 * @brief Creates a Non-Deterministic Finite Automaton (NFA)
 *        from a text file and generates its visual representation.
 *
 * This function reads an automaton definition from a text file
 * located in the `inputs/` directory, constructs an `Automaton`
 * object representing the NFA, and then generates a corresponding
 * DOT file and image visualization.
 *
 * @details
 * **Workflow:**
 *  1. Reads the automaton definition from:
 *       "../../inputs/<inputBaseName>.txt"
 *  2. Constructs an `Automaton` object using `Automaton::readAutomaton()`.
 *  3. Generates a `.dot` file describing the automaton’s structure:
 *       "../../dots/nfa_<inputBaseName>.dot"
 *  4. Uses the `Dot` utility to create an image (e.g., PNG/SVG)
 *     showing the NFA graphically.
 *  5. Returns the constructed NFA as an `Automaton` object.
 *
 * @param inputBaseName
 *        Base name of the automaton input file (without extension).
 *        For example, if `inputBaseName = "nfa1"`, the function reads:
 *          - Input: "../../inputs/nfa1.txt"
 *          - Output DOT: "../../dots/nfa_nfa1.dot"
 *
 * @return
 *        An `Automaton` object representing the NFA described in the file.
 *
 * @note
 * - The text file should follow the expected automaton input format
 *   (e.g., states, alphabet, transitions, initial/final states).
 * - This function is typically the *first step* in processing pipelines
 *   like determinisation, minimization, or regex-to-NFA conversions.
 */
Automaton createNonDeterministicAutomaton(const string& inputBaseName) {
    // Step 1: Build full path to the automaton input text file.
    // Example: "../../inputs/myNFA.txt"
    string inputTextFilePath = "../../inputs/" + inputBaseName + ".txt";

    // Step 2: Define path for output DOT file (for visualization).
    // Example: "../../dots/nfa_myNFA.dot"
    string outputDotFilePath = "../../dots/nfa_" + inputBaseName + ".dot";

    // Step 3: Read and construct the NFA from the input text file.
    // The Automaton class handles parsing the structure (states, transitions, etc.).
    Automaton nonDeterministicAutomaton = Automaton::readAutomaton(inputTextFilePath);

    // Step 4: Create a Dot utility instance for visualization.
    Dot dotGenerator;

    // Step 5: Generate the DOT file describing the automaton graph.
    // The label "NFA" will appear in the graph title.
    dotGenerator.generateDot(inputTextFilePath, outputDotFilePath, "NFA");

    // Step 6: Generate an image (e.g., PNG or SVG) from the DOT file.
    // The output image will be named like "nfa_<inputBaseName>.png".
    dotGenerator.generateImage(outputDotFilePath, "nfa_", inputBaseName);

    // Step 7: Return the constructed NFA for further processing.
    return nonDeterministicAutomaton;
}
