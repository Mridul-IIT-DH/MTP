#include "../include/Automaton.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/**
 * @brief Reads an automaton description from a text file and constructs
 *        an Automaton object from it.
 *
 * @details
 * The expected input format consists of the following labeled sections:
 *
 *    STATES:
 *       s1 s2 s3 ...
 *
 *    ALPHABET:
 *       a b c ...
 *
 *    TRANSITIONS:
 *       u a v
 *       x b y
 *       ...
 *
 *    INITIAL_STATES:
 *       s0
 *
 *    FINAL_STATES:
 *       f1 f2 ...
 *
 * Each section is followed by one or more lines of data, and an empty line
 * marks the end of that section. The parser ignores blank lines and reads
 * until EOF.
 *
 * Example:
 *    STATES:
 *    0 1 2
 *
 *    ALPHABET:
 *    a b
 *
 *    TRANSITIONS:
 *    0 a 1
 *    1 b 2
 *
 *    INITIAL_STATES:
 *    0
 *
 *    FINAL_STATES:
 *    2
 *
 * Any number of states, transitions, or alphabet symbols is supported.
 * Undeclared states in transitions are *not* automatically added—input must
 * be consistent.
 *
 * @param filename  Path to the automaton file (e.g., "../../inputs/dfa1.txt").
 *
 * @return Automaton object populated with parsed states, alphabet,
 *         transitions, initial states, and final states.
 *
 * @note
 * - No strict error validation is performed: malformed lines may produce
 *   partially parsed results.
 * - Alphabet symbols are assumed to be single characters.
 * - ε-transitions should be represented using '#' if applicable.
 */
Automaton Automaton::readAutomaton(const string& filename) {
    fstream fin(filename);

    Automaton A;

    string line;
    string label;

    // ---------------------------------------------------------------------
    // Read file line-by-line, using section labels to determine how lines
    // should be interpreted.
    // ---------------------------------------------------------------------
    while (getline(fin, line)) {

        if (line.empty())
            continue;   // skip blank lines

        stringstream ss(line);
        ss >> label;

        // --------------------------------------------------------------
        // STATES:
        // --------------------------------------------------------------
        if (label == "STATES:") {
            int s;
            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);
                while (ss2 >> s) {
                    A.states.insert(s);
                }
            }
        }

        // --------------------------------------------------------------
        // ALPHABET:
        // --------------------------------------------------------------
        else if (label == "ALPHABET:") {
            char c;
            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);
                while (ss2 >> c) {
                    A.alphabet.insert(c);
                }
            }
        }

        // --------------------------------------------------------------
        // TRANSITIONS:
        // Format: <from> <symbol> <to>
        // --------------------------------------------------------------
        else if (label == "TRANSITIONS:") {
            int from, to;
            char c;

            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);
                ss2 >> from >> c >> to;

                // Insert into transition map
                A.transitions[{from, c}].insert(to);
            }
        }

        // --------------------------------------------------------------
        // INITIAL_STATES:
        // --------------------------------------------------------------
        else if (label == "INITIAL_STATES:") {
            int s;
            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);
                while (ss2 >> s) {
                    A.initialStates.insert(s);
                }
            }
        }

        // --------------------------------------------------------------
        // FINAL_STATES:
        // --------------------------------------------------------------
        else if (label == "FINAL_STATES:") {
            int s;
            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);
                while (ss2 >> s) {
                    A.finalStates.insert(s);
                }
            }
        }
    }

    fin.close();
    return A;
}
