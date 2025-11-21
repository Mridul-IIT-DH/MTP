#include "../include/Automaton.h"
#include "../include/RegexENFA.h"
#include "../include/NFAToRegex.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Forward declarations (defined elsewhere)
Automaton eNFAtoNFA(const string& inputBaseName);
void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName);

/**
 * @brief Produces a **standardized (canonical) regular expression** for a given regex.
 *
 * @details
 * This function applies the following full pipeline:
 *
 *   --------------------------------------------------------------------------
 *   (1) Regex → ε-NFA  
 *       Uses Thompson’s construction (regexToENFA).  
 *       Produces an ε-NFA that accepts exactly the language of the regex.
 *
 *   (2) ε-NFA → NFA  
 *       Removes ε-transitions via epsilon-closure.
 *
 *   (3) NFA → Minimal DFA  
 *       Uses determinisation + DFA minimization.
 *       The minimal DFA is unique up to isomorphism.
 *
 *   (4) Minimal DFA → Regular Expression  
 *       Uses state-elimination (automatonToRegex).
 *       Because the input DFA is minimal, the resulting regex is a
 *       **canonical standardized representation** of the language.
 *
 *   --------------------------------------------------------------------------
 *   Why this produces a "standardized" regex:
 *
 *     • Different regexes may represent the same language.
 *     • Converting to a minimal DFA yields a unique structure (up to renaming).
 *     • Converting that minimal DFA back to regex yields a canonical form.
 *
 *   This ensures two different regexes for the same language produce the same
 *   final standardized regex.
 *
 *   --------------------------------------------------------------------------
 *   Output:
 *      The standardized regex is written to:
 *          "../../outputs/std_regex_<name>.txt"
 *
 * @note
 * - The regex file must contain a single-line regular expression.
 * - This function performs multiple read/write cycles to verify correctness.
 */
void standardizeRegex() {
    string regexBaseName;

    // --------------------------------------------------------------
    // Step 0: Get input regex name from user
    // --------------------------------------------------------------
    cout << "\nEnter the regex file name (without .txt): ";
    cin >> regexBaseName;

    // --------------------------------------------------------------
    // Step 1: Regex → ε-NFA
    // --------------------------------------------------------------
    cout << "Step 1: Converting Regex to eNFA..." << endl;
    Automaton enfa = regexToENFA(regexBaseName);

    // --------------------------------------------------------------
    // Step 2: ε-NFA → NFA (remove epsilon transitions)
    // --------------------------------------------------------------
    cout << "Step 2: Converting eNFA to NFA..." << endl;
    Automaton nfa = eNFAtoNFA(regexBaseName);

    // --------------------------------------------------------------
    // Step 3: Determinisation + Minimization
    //         Produces the minimal DFA, which is canonical.
    // --------------------------------------------------------------
    cout << "Step 3 & 4: Determinising, Minimizing, and generating image..." << endl;
    minimalDFA(nfa, regexBaseName);

    // --------------------------------------------------------------
    // Step 4 (continued): Read back the minimal DFA for conversion
    // --------------------------------------------------------------
    string minDFA_path = "../../outputs/min_" + regexBaseName + ".txt";
    cout << "Reading back minimal DFA from " << minDFA_path << "..." << endl;

    Automaton minDFA = Automaton::readAutomaton(minDFA_path);

    if (minDFA.getStates().empty()) {
        cout << "Error: Could not read back minimal DFA from " << minDFA_path << endl;
        return;
    }

    // --------------------------------------------------------------
    // Step 5: Minimal DFA → Standardized regex
    //         (Unique modulo trivial variations)
    // --------------------------------------------------------------
    cout << "Step 5: Converting Minimal DFA back to Regex..." << endl;
    string standardRegex = automatonToRegex(minDFA);

    // --------------------------------------------------------------
    // Step 6: Write standardized regex to output file
    // --------------------------------------------------------------
    string outputFilePath = "../../outputs/std_regex_" + regexBaseName + ".txt";
    ofstream fout(outputFilePath);

    if (fout.is_open()) {
        fout << standardRegex;
        fout.close();
        cout << "\nStandardized regex written to: " << outputFilePath << endl;
    } else {
        cout << "\nError: Unable to write to output file " << outputFilePath << endl;
    }

    // --------------------------------------------------------------
    // Step 7: Verify and display the final standardized regex
    // --------------------------------------------------------------
    cout << "\n--- Verifying file contents ---" << endl;

    string fileContents;
    ifstream fin(outputFilePath);

    if (fin.is_open()) {
        stringstream ss;
        ss << fin.rdbuf();
        fileContents = ss.str();
        fin.close();
    } else {
        cout << "Error: Could not read back file " << outputFilePath << endl;
    }

    // User-friendly interpretation of special cases
    if (fileContents.empty()) {
        cout << "Resulting language is the EMPTY SET (accepts no strings)." << endl;
    }
    else if (fileContents == "#") {
        cout << "Standardized regex (accepts only the empty string): #" << endl;
    }
    else {
        cout << "Standardized regex: " << fileContents << endl;
    }
}
