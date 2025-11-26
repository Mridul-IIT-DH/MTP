#include "../include/Automaton.h"
#include "../include/Dot.h"
#include "../include/RegexENFA.h"
#include "../include/NFAToRegex.h"
#include "../include/RegexUtils.h"
#include "../include/minimizeRegexFile.h"

#include <iostream>
#include <string>

using namespace std;

/*
 * Forward declaration for the menu print function.
 * Implemented in display.cpp.
 */
void display();

/*
 * Function prototypes for the different features available
 * in the menu. Each is implemented in its own .cpp file.
 */
Automaton createNonDeterministicAutomaton(const string& inputBaseName);

void proposition313(const Automaton& nonDeterministicAutomaton, const string& inputBaseName);
void brozozowskisAlgorithm(const Automaton& nonDeterministicAutomaton, const string& inputBaseName);
void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void checkIsomorphism();
void regexToMinimalDFA();
void automatonToImage(const string& inputBaseName);
void nfaToRegex();
void standardizeRegex();

/*
 * main()
 *
 * Main program loop for the automata toolkit.
 * Continuously displays a menu, reads user input, and dispatches
 * to the appropriate feature. The loop terminates when the user
 * selects option 0.
 *
 * Notes:
 *  - All heavy functionality is implemented in dedicated modules.
 *  - This file remains the high-level dispatcher only.
 *  - InputBaseName is reused for different operations.
 */
int main() {
    string inputBaseName;

    while (true) {
        display();

        cout << "Enter your choice: ";
        int choice = -1;
        cin >> choice;

        /*
         * If the user types a non-integer, the stream enters a fail state.
         * Clear the state and discard the rest of the line.
         */
        if (!cin) {
            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "Invalid input. Please enter a number corresponding to the menu options.\n";
            continue;
        }

        /*
         * 0 → exit the program.
         */
        if (choice == 0) {
            cout << "\n/////////////////////////\n";
            cout << "\nThank you for being here.\n";
            cout << "\n/////////////////////////\n";
            cout << endl;
            return 0;
        }

        /*
         * 1 → Proposition 3.13
         */
        else if (choice == 1) {
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;

            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            proposition313(nonDeterministicAutomaton, inputBaseName);
        }

        /*
         * 2 → Brzozowski's Algorithm
         */
        else if (choice == 2) {
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;

            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            brozozowskisAlgorithm(nonDeterministicAutomaton, inputBaseName);
        }

        /*
         * 3 → Determinise then minimize
         */
        else if (choice == 3) {
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;

            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            minimalDFA(nonDeterministicAutomaton, inputBaseName);
        }

        /*
         * 4 → Run options 1, 2, and 3 in sequence
         */
        else if (choice == 4) {
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;

            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);

            proposition313(nonDeterministicAutomaton, inputBaseName);
            brozozowskisAlgorithm(nonDeterministicAutomaton, inputBaseName);
            minimalDFA(nonDeterministicAutomaton, inputBaseName);
        }

        /*
         * 5 → Check if two minimized DFAs are isomorphic
         */
        else if (choice == 5) {
            checkIsomorphism();
        }

        /*
         * 6 → Regex to minimal DFA
         */
        else if (choice == 6) {
            regexToMinimalDFA();
        }

        /*
         * 7 → Generate image of an automaton
         */
        else if (choice == 7) {
            cout << "\nEnter the input automaton file name (without .txt): ";
            cin >> inputBaseName;
            automatonToImage(inputBaseName);
        }

        /*
         * 8 → NFA to regex conversion
         */
        else if (choice == 8) {
            nfaToRegex();
        }

        /*
         * 9 → Standardize a regular expression
         */
        else if (choice == 9) {
            standardizeRegex();
        }

        /*
         * 10 → Minimize regex from file (AST normalization + DFA canonicalization)
         */
        else if (choice == 10) {
            minimizeRegexFromFile();
        }

        /*
         * Any unknown option → Invalid
         */
        else {
            cout << "Invalid input! Please select a valid option from the menu." << endl;
        }
    }

    return 0;
}
