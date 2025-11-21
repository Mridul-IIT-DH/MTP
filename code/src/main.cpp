#include "../include/Automaton.h"
#include "../include/Dot.h"
#include "../include/RegexENFA.h"
#include "../include/NFAToRegex.h"

#include <iostream>
#include <string>

using namespace std;

/*
 * main.cpp
 *
 * Program entry point and high-level CLI dispatcher for the automata toolkit.
 *
 * This file wires together the various modules (propositions, conversions,
 * minimization algorithms, visualization tools, regex utilities, etc.) and
 * provides a simple console menu for the user to pick operations.
 *
 * Design goals:
 *  - Keep user interaction logic separate from algorithmic implementations.
 *  - Keep main() small and readable: collect input, call the appropriate
 *    module function, and report results.
 *  - All heavy-lifting (parsing, determinisation, minimization, DOT generation)
 *    is performed in other translation units.
 *
 * Notes:
 *  - The function prototypes below are forward declarations for functions
 *    implemented in other compilation units (one per feature).
 *  - Filenames requested from the user are expected without the ".txt" extension.
 *  - createNonDeterministicAutomaton(...) both reads an automaton from the inputs
 *    folder and generates a visualization; many other functions accept the
 *    resulting Automaton object as input.
 */

/* Forward declarations for functions implemented in other .cpp files */
void display();  // Prints the main menu (display.cpp)

/* Utility / pipeline functions */
Automaton createNonDeterministicAutomaton(const string& inputBaseName); // createNonDeterministicAutomaton.cpp

/* Core feature functions */
void proposition313(const Automaton& nonDeterministicAutomaton, const string& inputBaseName); // proposition313.cpp
void brozozowskisAlgorithm(const Automaton& nonDeterministicAutomaton, const string& inputBaseName); // brozozowskisAlgorithm.cpp
void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName); // minimalDFA.cpp / minimize.cpp

/* Other utilities */
void checkIsomorphism();      // checkIsomorphism.cpp
void regexToMinimalDFA();    // regexToMinimalDFA.cpp
void automatonToImage(const string& inputBaseName); // automatonToImage.cpp
void nfaToRegex();           // NFAToRegex.cpp
void standardizeRegex();     // standardizeRegex.cpp

int main() {
    string inputBaseName;

    // Main program loop: show menu, read choice, dispatch to appropriate routine.
    // Loop continues until the user selects the Exit option (choice == 0).
    while (true) {
        display();  // Print the menu options to the console

        cout << "Enter your choice: ";
        int choice = -1;
        cin >> choice;

        // Basic input validation: if stream is in a failed state, clear and continue.
        if (!cin) {
            cin.clear();
            string junk;
            getline(cin, junk); // discard the rest of the invalid line
            cout << "Invalid input. Please enter a number corresponding to the menu options.\n";
            continue;
        }

        // Dispatch based on the user's selection.
        if (choice == 0) {
            // Exit the program gracefully with a short goodbye message.
            cout << "\n/////////////////////////\n";
            cout << "\nThank you for being here.\n";
            cout << "\n/////////////////////////\n";
            cout << endl;
            return 0;
        }
        else if (choice == 1) {
            // Proposition 3.13 (whatever the project defines as proposition313)
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;

            // Read and visualize the NFA/DFA, then run the proposition313 routine.
            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            proposition313(nonDeterministicAutomaton, inputBaseName);
        }
        else if (choice == 2) {
            // Brzozowski's algorithm (minimization via reverse+determinise twice)
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;

            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            brozozowskisAlgorithm(nonDeterministicAutomaton, inputBaseName);
        }
        else if (choice == 3) {
            // Determinise then minimize (produce a minimal DFA using subset construction
            // followed by a minimization algorithm implemented elsewhere).
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;

            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            minimalDFA(nonDeterministicAutomaton, inputBaseName);
        }
        else if (choice == 4) {
            // Run options 1, 2 and 3 in sequence for the same input automaton.
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;

            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);

            // Run the three selected workflows one after another.
            proposition313(nonDeterministicAutomaton, inputBaseName);
            brozozowskisAlgorithm(nonDeterministicAutomaton, inputBaseName);
            minimalDFA(nonDeterministicAutomaton, inputBaseName);
        }
        else if (choice == 5) {
            // Interactive check for isomorphism between two minimized DFAs.
            checkIsomorphism();
        }
        else if (choice == 6) {
            // Convert a regular expression to a minimal DFA (pipeline in regexToMinimalDFA()).
            regexToMinimalDFA();
        }
        else if (choice == 7) {
            // Generate an image for a named automaton text file.
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;
            automatonToImage(inputBaseName);
        }
        else if (choice == 8) {
            // Convert an NFA to a Regular Expression (state-elimination or other method).
            nfaToRegex();
        }
        else if (choice == 9) {
            // Standardize or normalize a regular expression (project-specific behavior).
            standardizeRegex();
        }
        else {
            // Any other numeric input is invalid: inform the user and re-display menu.
            cout << "Invalid input! Please select a valid option from the menu." << endl;
        }
    }

    // Unreachable, but keep a return for completeness.
    return 0;
}
