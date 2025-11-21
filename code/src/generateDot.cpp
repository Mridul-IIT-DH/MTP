#include "../include/Dot.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

/**
 * @brief Generates a Graphviz DOT file representing the structure
 *        of an automaton described in a text file.
 *
 * @details
 * This function reads an automaton definition from a text file containing
 * the following sections:
 *
 *   TRANSITIONS:
 *      <from> <symbol> <to>
 *      ...
 *
 *   INITIAL_STATES:
 *      s1 s2 ...
 *
 *   FINAL_STATES:
 *      f1 f2 ...
 *
 * Using this information, it produces a DOT file that Graphviz can render
 * into an image. The generated graph uses:
 *   - Left-to-right layout (rankdir=LR)
 *   - Double circles for final states
 *   - A small "start" point node with edges to initial states
 *   - Labeled edges for transitions
 *
 * The DOT file can then be rendered into PNG/SVG using Dot::generateImage().
 *
 * @param inputFile   Path to the automaton text file.
 * @param outputFile  Path where the DOT file should be written.
 * @param graphName   Name assigned to the DOT graph.
 */
void Dot::generateDot(const string& inputFile, const string& outputFile, const string& graphName) {
    string line;

    ifstream fin(inputFile);

    // Containers for parsed automaton data
    vector<tuple<int, char, int>> transitions;   // (from, symbol, to)
    vector<int> initialStates;
    vector<int> finalStates;

    // ---------------------------------------------------------------
    // Step 1: Read and parse the automaton file.
    // ---------------------------------------------------------------
    while (getline(fin, line)) {
        // --- TRANSITIONS section ---
        if (line == "TRANSITIONS:") {
            // Read transitions until a blank line or next section
            while (getline(fin, line) && !line.empty()) {
                stringstream ss(line);

                int from;
                char c;
                int to;

                ss >> from >> c >> to;
                transitions.push_back({from, c, to});
            }
        }
        // --- INITIAL STATES section ---
        else if (line == "INITIAL_STATES:") {
            if (getline(fin, line)) {
                stringstream ss(line);
                int temp;

                while (ss >> temp) {
                    initialStates.push_back(temp);
                }
            }
        }
        // --- FINAL STATES section ---
        else if (line == "FINAL_STATES:") {
            if (getline(fin, line)) {
                stringstream ss(line);
                int temp;

                while (ss >> temp) {
                    finalStates.push_back(temp);
                }
            }
        }
    }

    // ---------------------------------------------------------------
    // Step 2: Write the DOT file describing the automaton graph.
    // ---------------------------------------------------------------
    ofstream fout(outputFile);

    // Begin DOT graph
    fout << "digraph " << graphName << " {"
         << "\n\t" << "rankdir=LR;"      // Draw graph left-to-right
         << "\n\t" << "size=\"8,5\";"    // Default size
         << "\n\n";

    // --- Final states drawn as double circles ---
    fout << "\t" << "node [shape = doublecircle];\n\t";
    for (int finalState : finalStates) {
        fout << finalState << " ";
    }
    fout << ";\n";

    // --- Reset node style to single circle for all others ---
    fout << "\t" << "node [shape = circle];\n";

    // --- Start point (invisible node) pointing to initial states ---
    fout << "\t" << "start [shape=point];\n\t";
    for (int initialState : initialStates) {
        fout << "start -> " << initialState << ";\n\t";
    }

    // --- Draw transitions ---
    fout << "\n\t";
    for (auto& transition : transitions) {
        fout << get<0>(transition)
             << " -> "
             << get<2>(transition)
             << " [label=\""
             << get<1>(transition)
             << "\"];\n\t";
    }

    // End DOT graph
    fout << "\n}";
}
