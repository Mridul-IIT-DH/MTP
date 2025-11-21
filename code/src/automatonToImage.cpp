#include "../include/Dot.h"

#include <string>

using namespace std;

/**
 * @brief Converts a text-based automaton description into a visual image.
 *
 * This function takes the name of an input automaton (without extension),
 * reads its textual definition from the `inputs/` directory, and uses the
 * `Dot` utility class to:
 *   1. Generate a Graphviz DOT file representing the automaton structure.
 *   2. Render that DOT file into an image (e.g., PNG or SVG) for visualization.
 *
 * Typical usage:
 *   automatonToImage("dfa_example");
 *
 * will:
 *   - Read from: "../../inputs/dfa_example.txt"
 *   - Write DOT to: "../../dots/automaton_dfa_example.dot"
 *   - Produce image: "../../dots/automaton_dfa_example.png" (via Dot::generateImage)
 *
 * @param inputBaseName  Base name of the automaton input file (no extension).
 *                       The function expects the automaton description file to
 *                       be named "<inputBaseName>.txt" in the `inputs/` folder.
 */
void automatonToImage(const string& inputBaseName) {
    // Construct the full path to the automaton input file.
    // Example: "../../inputs/myDFA.txt"
    string inputTextFilePath = "../../inputs/" + inputBaseName + ".txt";

    // Construct the output path for the intermediate DOT file.
    // Example: "../../dots/automaton_myDFA.dot"
    string outputDotFilePath = "../../dots/automaton_" + inputBaseName + ".dot";

    // Create an instance of the Dot utility class, which handles
    // generating Graphviz DOT files and converting them to images.
    Dot dotGenerator;

    // Step 1: Generate a DOT representation of the automaton.
    // This step reads the text file and produces a .dot file with nodes,
    // edges, and labels describing the automaton’s structure.
    dotGenerator.generateDot(inputTextFilePath, outputDotFilePath, "Automaton");

    // Step 2: Generate an image (e.g., PNG) from the DOT file for visualization.
    // The output image will be named with prefix "automaton_" and the same base name.
    dotGenerator.generateImage(outputDotFilePath, "automaton_", inputBaseName);

    // Function completed successfully.
    return;
}
