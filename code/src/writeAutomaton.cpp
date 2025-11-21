#include "../include/Automaton.h"

#include <fstream>

/**
 * @brief Writes the automaton to a text file in a standardized, readable format.
 *
 * @details
 * The output format is intentionally symmetric with readAutomaton(), consisting of:
 *
 *    STATES:
 *        q1 q2 q3 ...
 *
 *    ALPHABET:
 *        a b c ...
 *
 *    TRANSITIONS:
 *        u a v
 *        x b y
 *        ...
 *
 *    INITIAL_STATES:
 *        i1 i2 ...
 *
 *    FINAL_STATES:
 *        f1 f2 ...
 *
 * Notes on the format:
 *  - States and alphabet symbols are space-separated.
 *  - Each transition is printed on its own line as: <from> <symbol> <to>
 *  - Sections are separated by blank lines for readability.
 *  - ε-transitions (if any) are written using '#'.
 *  - The order of states, transitions, and symbols is not guaranteed, but is
 *    deterministic based on the underlying std::set and map ordering.
 *
 * This format is used throughout the project for:
 *  - Storing intermediate automata (ENFA, NFA, DFA, minimal DFA)
 *  - Loading automata later for conversion or visualization
 *  - Maintaining compatibility with Dot generator modules
 *
 * @param filename  Output file path (e.g., "../../outputs/min_A.txt")
 */
void Automaton::writeAutomaton(const std::string &filename) const {
    std::ofstream fout(filename);

    // --------------------------------------------------------------
    // STATES
    // --------------------------------------------------------------
    fout << "STATES:\n";
    for (int s : states) {
        fout << s << " ";
    }
    fout << "\n\n";

    // --------------------------------------------------------------
    // ALPHABET
    // --------------------------------------------------------------
    fout << "ALPHABET:\n";
    for (char c : alphabet) {
        fout << c << " ";
    }
    fout << "\n\n";

    // --------------------------------------------------------------
    // TRANSITIONS
    // Format: <from> <symbol> <to>
    // --------------------------------------------------------------
    fout << "TRANSITIONS:\n";
    for (auto &p : transitions) {
        for (int to : p.second) {
            fout << p.first.first << " " << p.first.second << " " << to << "\n";
        }
    }
    fout << "\n";

    // --------------------------------------------------------------
    // INITIAL STATES
    // --------------------------------------------------------------
    fout << "INITIAL_STATES:\n";
    for (int s : initialStates) {
        fout << s << " ";
    }
    fout << "\n\n";

    // --------------------------------------------------------------
    // FINAL STATES
    // --------------------------------------------------------------
    fout << "FINAL_STATES:\n";
    for (int s : finalStates) {
        fout << s << " ";
    }
    fout << "\n";

    fout.close();
}
