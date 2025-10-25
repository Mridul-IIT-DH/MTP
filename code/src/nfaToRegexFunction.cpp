#include "../include/Automaton.h"
#include "../include/NFAToRegex.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void nfaToRegex() {
    string nfaBaseName;
    cout << "\nEnter the NFA file name (from inputs folder, without .txt): ";
    cin >> nfaBaseName;

    string nfaInputPath;
    if (nfaBaseName.rfind("nfa_", 0) == 0 || nfaBaseName.rfind("enfa_", 0) == 0 || nfaBaseName.rfind("min_", 0) == 0 || nfaBaseName.rfind("pro_", 0) == 0 || nfaBaseName.rfind("bro_", 0) == 0) {
        cout << "(Reading from inputs folder...)" << endl;
        nfaInputPath = "../../inputs/" + nfaBaseName + ".txt";
    } else {
        cout << "(Reading from inputs folder...)" << endl;
        nfaInputPath = "../../inputs/" + nfaBaseName + ".txt";
    }
    
    Automaton nfa = Automaton::readAutomaton(nfaInputPath);
    if (nfa.getStates().empty()) {
        cout << "Error: Could not read automaton from " << nfaInputPath << endl;
        return;
    }

    cout << "Converting Automaton to Regular Expression..." << endl;
    string regex = automatonToRegex(nfa);

    string outputFilePath = "../../outputs/regex_" + nfaBaseName + ".txt";
    ofstream fout(outputFilePath);
    if (fout.is_open()) {
        fout << regex;
        fout.close();
        cout << "\nConverted regex written to: " << outputFilePath << endl;
    } else {
        cout << "\nError: Unable to write to output file " << outputFilePath << endl;
    }

    if (regex.empty()) {
        cout << "Resulting language is the EMPTY SET (accepts no strings)." << endl;
    } else if (regex == "#") {
        cout << "Resulting regex (accepts only the empty string): #" << endl;
    } else {
        cout << "Resulting regex: " << regex << endl;
    }
}