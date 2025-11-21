#ifndef DOT_H
#define DOT_H

#include "Automaton.h"

#include <map>
#include <string>

/**
 * @class Dot
 *
 * @brief Utility class for generating Graphviz DOT files and PNG images
 *        for visualizing automata.
 *
 * @details
 * The Dot class is responsible for converting automata (or pairs of
 * automata) into visual diagrams using the Graphviz DOT language.
 *
 * It provides:
 *
 *   • generateDot() – Convert an automaton text file → DOT graph
 *   • generateImage() – Convert DOT graph → PNG using Graphviz command-line
 *   • generateIsomorphismDot() – Visualize two automata side-by-side
 *       with isomorphism mapping edges
 *
 * All images are written to:
 *      ../../images/
 *
 * All DOT files are written to:
 *      ../../dots/
 *
 * This class does not interpret automata directly except in the isomorphism
 * visualization function — it generally converts already existing text
 * automaton descriptions into DOT syntax.
 */
class Dot {
public:

    /**
     * @brief Generates a Graphviz DOT file from an automaton description file.
     *
     * @details
     * Input:
     *     A text file (in the Automaton::writeAutomaton format)
     *     describing states, transitions, initial and final states.
     *
     * Output:
     *     A DOT graph containing:
     *       - Nodes for each state
     *       - Directed edges for transitions
     *       - Double-circle shape for final states
     *       - A dummy "start" arrow pointing to each initial state
     *
     * @param inputFilename  Path to automaton text file.
     * @param outputFilename Path where DOT file should be written.
     * @param graphName      Name of the DOT graph.
     */
    void generateDot(const std::string& inputFilename,
                     const std::string& outputFilename,
                     const std::string& graphName);

    /**
     * @brief Converts a DOT file into a PNG image using Graphviz.
     *
     * @details
     * Internally executes:
     *     dot -Tpng <dotfile> -o <outputfile>
     *
     * Images are stored in:
     *     ../../images/prefix + filename + ".png"
     *
     * @param file      Path to the DOT file.
     * @param prefix    Prefix for output filename.
     * @param filename  Base name of the image file.
     */
    void generateImage(const std::string& file,
                       const std::string& prefix,
                       const std::string& filename);

    /**
     * @brief Generates a DOT visualization showing the isomorphism
     *        between two automata A and B.
     *
     * @details
     * Produces a combined graph containing:
     *   • Subgraph cluster for Automaton A
     *   • Subgraph cluster for Automaton B
     *   • Matching states connected with dashed blue arrows
     *     representing the isomorphism mapping:
     *           A(q)  ———→  B(f(q))
     *
     * This diagram clearly displays structural correspondence between
     * two minimal DFAs.
     *
     * Output files:
     *     ../../dots/iso_<filenameBase>.dot
     *     ../../images/iso_<filenameBase>.png
     *
     * @param A            First automaton.
     * @param B            Second automaton.
     * @param mapping      Bijection f: states(A) → states(B).
     * @param filenameBase Base name used for DOT and PNG files.
     */
    void generateIsomorphismDot(
        const Automaton& A,
        const Automaton& B,
        const std::map<int, int>& mapping,
        const std::string& filenameBase
    );
};

#endif
