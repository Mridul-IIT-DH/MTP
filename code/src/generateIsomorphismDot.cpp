#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <fstream>
#include <map>
#include <string>
#include <iostream>

using namespace std;

/**
 * @brief Generates a DOT graph visualizing the isomorphism between
 *        two minimized DFAs A and B.
 *
 * @details
 * This function creates a side-by-side visualization of two DFAs,
 * highlighting how each state in Automaton A corresponds to a state
 * in Automaton B according to an isomorphism mapping.
 *
 * The graph contains:
 *   - Two clusters: one for Automaton A and one for Automaton B.
 *   - Nodes representing each state in A and B.
 *   - Final states colored (e.g., palegreen).
 *   - Invisible start-point markers pointing to initial states.
 *   - Dashed blue arrows connecting A's states to their mapped
 *     states in B, labeled with "≈" (meaning “corresponds to”).
 *
 * Output:
 *   - DOT file:  ../../dots/iso_<filenameBase>.dot
 *   - PNG file:  ../../images/iso_<filenameBase>.png
 *
 * @param A            The first automaton.
 * @param B            The second automaton.
 * @param mapping      A state-to-state mapping: A_state → B_state.
 * @param filenameBase Base name used to generate output file names.
 */
void Dot::generateIsomorphismDot(
    const Automaton& A,
    const Automaton& B,
    const map<int, int>& mapping,
    const string& filenameBase
) {
    // Construct output file paths
    string dotFile = "../../dots/iso_" + filenameBase + ".dot";
    string pngFile = "../../images/iso_" + filenameBase + ".png";

    ofstream fout(dotFile);
    if (!fout.is_open()) {
        cerr << "Error: Unable to open " << dotFile << " for writing.\n";
        return;
    }

    // -------------------------------------------------------
    // Start of DOT graph
    // -------------------------------------------------------
    fout << "digraph Isomorphism {"
         << "\n\t"
         << "rankdir=LR;"                         // Left to right layout
         << "\n\t"
         << "node [shape=circle, style=filled, fillcolor=white];"
         << "\n\t"
         << "splines=true;";                      // Allow curved edges

    // -------------------------------------------------------
    // Subgraph: Automaton A
    // -------------------------------------------------------
    fout << "\n\t"
         << "subgraph cluster_A {"
         << "\n\t\t"
         << "label=\"Automaton A\";"              // Cluster label
         << "\n\t\t"
         << "color=lightgrey;";                   // Cluster boundary

    // For each state in A, create a node "A<state>"
    for (int s : A.getStates()) {
        bool isFinal = A.getFinalStates().count(s);
        string color = isFinal ? "palegreen" : "white";

        fout << "\n\t\t"
             << "A" << s
             << " [label=\"A" << s << "\", fillcolor=\"" << color << "\"];";
    }
    fout << "\n\t}";  // End cluster_A

    // -------------------------------------------------------
    // Subgraph: Automaton B
    // -------------------------------------------------------
    fout << "\n\t"
         << "subgraph cluster_B {"
         << "\n\t\t"
         << "label=\"Automaton B\";"
         << "\n\t\t"
         << "color=lightgrey;";

    // Each state in B represented as "B<state>"
    for (int s : B.getStates()) {
        bool isFinal = B.getFinalStates().count(s);
        string color = isFinal ? "palegreen" : "white";

        fout << "\n\t\t"
             << "B" << s
             << " [label=\"B" << s << "\", fillcolor=\"" << color << "\"];";
    }
    fout << "\n\t}";  // End cluster_B

    // -------------------------------------------------------
    // Initial states: add invisible point nodes → arrows
    // -------------------------------------------------------
    fout << "\n\t";

    for (int s : A.getInitialStates()) {
        fout << "initA" << s << " [shape=point, style=invis];\n\t"
             << "initA" << s << " -> A" << s << " [color=black];\n\t";
    }
    for (int s : B.getInitialStates()) {
        fout << "initB" << s << " [shape=point, style=invis];\n\t"
             << "initB" << s << " -> B" << s << " [color=black];\n\t";
    }

    // -------------------------------------------------------
    // Draw dashed arrows representing isomorphism mapping
    // -------------------------------------------------------
    for (auto& [a, b] : mapping) {
        fout << "\n\t"
             << "A" << a << " -> B" << b
             << " [color=blue, style=dashed, label=\"≈\"];";
    }

    // -------------------------------------------------------
    // End of graph
    // -------------------------------------------------------
    fout << "\n}\n";
    fout.close();

    // Generate PNG image from DOT file using Graphviz
    string command = "dot -Tpng \"" + dotFile + "\" -o \"" + pngFile + "\"";
    system(command.c_str());
}
