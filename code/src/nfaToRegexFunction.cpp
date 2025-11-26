#include "../include/Automaton.h"
#include "../include/NFAToRegex.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Interactive wrapper that reads an NFA from the inputs folder
 *        and converts it into a regular expression using the
 *        state-elimination method (implemented in automatonToRegex()).
 *
 * @details
 * This function:
 *   1. Prompts the user for an input file base name (without .txt).
 *   2. Reads the automaton from "../../inputs/<name>.txt".
 *   3. Calls automatonToRegex(nfa) to compute the equivalent regex.
 *   4. Writes the resulting regex to:
 *         "../../outputs/regex_<name>.txt"
 *   5. Prints the regex to the console.
 *
 * Notes:
 *   - The prefix check (nfa_, enfa_, min_, etc.) currently does not
 *     alter behavior, but it establishes a naming convention for the user.
 *   - The regex returned may be:
 *         ""   → empty language
 *         "#"  → epsilon (empty string)
 *         other expressions such as (a|b)*a
 */
void nfaToRegex() {
    string nfaBaseName;

    // --------------------------------------------------------------
    // Step 1: Ask the user for the base name of the automaton file.
    // --------------------------------------------------------------
    cout << "\nEnter the NFA file name (from inputs folder, without .txt): ";
    cin >> nfaBaseName;

    // --------------------------------------------------------------
    // Step 2: Construct the full path to the input automaton
    // --------------------------------------------------------------
    cout << "(Reading from inputs folder...)" << endl;
    string nfaInputPath = "../../inputs/" + nfaBaseName + ".txt";

    // --------------------------------------------------------------
    // Step 3: Read the automaton from text file.
    // --------------------------------------------------------------
    Automaton nfa = Automaton::readAutomaton(nfaInputPath);

    if (nfa.getStates().empty()) {
        cout << "Error: Could not read automaton from " << nfaInputPath << endl;
        return;
    }

    // --------------------------------------------------------------
    // Step 4: Convert automaton → regular expression.
    //         (Using the state elimination algorithm.)
    // --------------------------------------------------------------
    cout << "Converting Automaton to Regular Expression..." << endl;

    string regex = automatonToRegex(nfa);

    // --------------------------------------------------------------
    // Step 5: Write resulting regex to the outputs folder.
    // --------------------------------------------------------------
    string outputFilePath = "../../outputs/regex_" + nfaBaseName + ".txt";
    ofstream fout(outputFilePath);

    if (fout.is_open()) {
        fout << regex;
        fout.close();
        cout << "\nConverted regex written to: " << outputFilePath << endl;
    } else {
        cout << "\nError: Unable to write to output file " << outputFilePath << endl;
    }

    // --------------------------------------------------------------
    // Step 6: Print the meaning of the resulting regex.
    // --------------------------------------------------------------
    if (regex.empty()) {
        cout << "Resulting language is the EMPTY SET (accepts no strings)." << endl;
    }
    else if (regex == "#") {
        cout << "Resulting regex (accepts only the empty string): #" << endl;
    }
    else {
        cout << "Resulting regex: " << regex << endl;
    }
}
