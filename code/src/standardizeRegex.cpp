#include "../include/Automaton.h"
#include "../include/RegexENFA.h"
#include "../include/NFAToRegex.h"

#include <fstream> 
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Automaton eNFAtoNFA(const string& inputBaseName);
void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void standardizeRegex() {
    string regexBaseName;

    cout << "\nEnter the regex file name (without .txt): ";
    cin >> regexBaseName;

    cout << "Step 1: Converting Regex to eNFA..." << endl;
    Automaton enfa = regexToENFA(regexBaseName);

    cout << "Step 2: Converting eNFA to NFA..." << endl;
    Automaton nfa = eNFAtoNFA(regexBaseName);

    cout << "Step 3 & 4: Determinising, Minimizing, and generating image..." << endl;
    minimalDFA(nfa, regexBaseName);

    string minDFA_path = "../../outputs/min_" + regexBaseName + ".txt";
    cout << "Reading back minimal DFA from " << minDFA_path << "..." << endl;
    Automaton minDFA = Automaton::readAutomaton(minDFA_path);
    
    if (minDFA.getStates().empty()) {
        cout << "Error: Could not read back minimal DFA from " << minDFA_path << endl;
        return;
    }

    cout << "Step 5: Converting Minimal DFA back to Regex..." << endl;
    string standardRegex = automatonToRegex(minDFA);

    string outputFilePath = "../../outputs/std_regex_" + regexBaseName + ".txt";
    ofstream fout(outputFilePath);
    if (fout.is_open()) {
        fout << standardRegex;
        fout.close();
        cout << "\nStandardized regex written to: " << outputFilePath << endl;
    } else {
        cout << "\nError: Unable to write to output file " << outputFilePath << endl;
    }

    cout << "\n--- Verifying file contents ---" << endl;
    ifstream fin(outputFilePath);
    string fileContents;
    if (fin.is_open()) {
        stringstream ss;
        ss << fin.rdbuf();
        fileContents = ss.str();
        fin.close();
    } else {
        cout << "Error: Could not read back file " << outputFilePath << endl;
    }

    if (fileContents.empty()) {
        cout << "Resulting language is the EMPTY SET (accepts no strings)." << endl;
    } else if (fileContents == "#") {
        cout << "Standardized regex (accepts only the empty string): #" << endl;
    } else {
        cout << "Standardized regex: " << fileContents << endl;
    }
}