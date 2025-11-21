#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <iostream>
#include <map>
#include <string>

using namespace std;

/**
 * @brief Interactively checks whether two minimized DFAs are isomorphic
 *        (i.e., structurally identical up to renaming of states).
 *
 * This function prompts the user for two filenames (without extensions),
 * reads both automata from the `outputs/` directory, and then uses
 * `Automaton::isIsomorphic()` to test for isomorphism.
 *
 * If the automata are found to be isomorphic:
 *   - It reports the result in the console.
 *   - Generates a DOT and image visualization that highlights the
 *     correspondence between states in both DFAs.
 *
 * If not isomorphic, it simply reports that result.
 *
 * @details
 * **Expected workflow:**
 *   1. User provides two minimized DFA names (e.g., "dfa1" and "dfa2").
 *   2. The function constructs file paths:
 *        - "../../outputs/dfa1.txt"
 *        - "../../outputs/dfa2.txt"
 *   3. Both automata are loaded using `Automaton::readAutomaton()`.
 *   4. Structural isomorphism is checked using `Automaton::isIsomorphic()`.
 *   5. If isomorphic, a visualization of the state mapping is generated
 *      (saved as "MTP/images/iso_<file1>_vs_<file2>.png").
 *
 * @note
 * - This function assumes both automata are **minimized DFAs**.
 *   Non-minimal or nondeterministic automata may not compare meaningfully.
 * - The visualization relies on the `Dot` utility class.
 */
void checkIsomorphism() {
    string file1;
    string file2;

    // --- Step 1: Prompt user for input filenames ---
    cout << "\nEnter the first minimized DFA filename (without .txt): ";
    cin >> file1;

    cout << "Enter the second minimized DFA filename (without .txt): ";
    cin >> file2;

    // --- Step 2: Construct full file paths for both automata ---
    // Automata are expected to be stored in the ../../outputs/ directory.
    string path1 = "../../outputs/" + file1 + ".txt";
    string path2 = "../../outputs/" + file2 + ".txt";

    // --- Step 3: Load automata from the specified text files ---
    // The Automaton class provides a static function to read automata
    // in a structured format (e.g., states, transitions, final states).
    Automaton A = Automaton::readAutomaton(path1);
    Automaton B = Automaton::readAutomaton(path2);

    // --- Step 4: Prepare a map to store the state correspondence (if found) ---
    // mapping[i] = j means state i in A corresponds to state j in B.
    map<int, int> mapping;

    cout << "\nChecking isomorphism between " << file1 << " and " << file2 << "...\n";

    // --- Step 5: Perform the isomorphism check ---
    // Uses the formal definition implemented in Automaton::isIsomorphic().
    if (Automaton::isIsomorphic(A, B, &mapping)) {
        cout << "The DFAs are isomorphic (equivalent up to renaming)." << endl;

        // --- Step 6: Generate visualization of the isomorphism ---
        // The Dot utility creates a combined diagram showing how
        // states from A correspond to states in B.
        Dot dotGenerator;
        dotGenerator.generateIsomorphismDot(A, B, mapping, file1 + "_vs_" + file2);

        cout << "Visualization generated: MTP/images/iso_" 
             << file1 << "_vs_" << file2 << ".png" << endl;
    } 
    else {
        // --- Step 7: Report non-isomorphism ---
        cout << "The DFAs are NOT isomorphic." << endl;
    }
}
