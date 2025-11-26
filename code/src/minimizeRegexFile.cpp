#include "../include/minimizeRegexFile.h"
#include "../include/RegexUtils.h"
#include "../include/Automaton.h"
#include "../include/RegexENFA.h"
#include "../include/Dot.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
 * minimizeRegexFromFile()
 *
 * Reads a regular expression from a .txt file in the inputs directory,
 * minimizes it using the complete regex→ENFA→NFA→DFA→minDFA→regex pipeline,
 * writes the minimized regex to the outputs directory, and also generates
 * an image of the minimal DFA corresponding to the minimized regex.
 *
 * This function is called from main.cpp under menu option 10.
 */
void minimizeRegexFromFile() {
    cout << "\nEnter the regex file name (from inputs folder, without .txt): ";
    string regexBaseName;
    cin >> regexBaseName;

    string inputPath  = "../../inputs/" + regexBaseName + ".txt";
    string outputPath = "../../outputs/min_regex_" + regexBaseName + ".txt";

    /*
     * Step 1: Read the raw regex from file
     */
    ifstream fin(inputPath);
    if (!fin.is_open()) {
        cout << "Error: Could not read file " << inputPath << endl;
        return;
    }

    string raw;
    getline(fin, raw);
    fin.close();

    cout << "Minimizing regex: " << raw << endl;

    /*
     * Step 2: Minimize the regex using the project's full pipeline
     */
    string minimized = minimizeRegex(raw);

    /*
     * Step 3: Write minimized regex to outputs directory
     */
    ofstream fout(outputPath);
    if (!fout.is_open()) {
        cout << "Error: Could not write to " << outputPath << endl;
        return;
    }
    fout << minimized;
    fout.close();

    cout << "\nMinimized regex written to: " << outputPath << endl;

    /*
     * Step 4: Write minimized regex into a temporary file so that
     * regexToENFA() and related modules (which expect filenames)
     * can be reused without modifying their interfaces.
     */
    string tmpRegexBase = "tmp_minimized_regex";
    string tmpRegexPath = "../../inputs/" + tmpRegexBase + ".txt";

    {
        ofstream tmp(tmpRegexPath);
        if (!tmp.is_open()) {
            cout << "Error: Could not write temporary regex file " << tmpRegexPath << endl;
            return;
        }
        tmp << minimized;
        tmp.close();
    }

    /*
     * Step 5: Run minimized regex through the automata pipeline:
     *         regex → ENFA → NFA → DFA → minimal DFA
     */
    Automaton enfa = regexToENFA(tmpRegexBase);
    Automaton nfa  = eNFAtoNFA(tmpRegexBase);

    Automaton dfa    = Automaton::determinise(nfa);
    Automaton minDFA = Automaton::minimize(dfa);

    /*
     * Step 6: Write minimal DFA to outputs directory
     */
    string autoOut = "../../outputs/min_regex_automaton_" + regexBaseName + ".txt";
    minDFA.writeAutomaton(autoOut);

    /*
     * Step 7: Generate DOT + PNG image of the minimal DFA
     */
    string dotOut = "../../dots/min_regex_automaton_" + regexBaseName + ".dot";

    Dot dotGen;
    dotGen.generateDot(autoOut, dotOut, "MinRegexDFA");
    dotGen.generateImage(dotOut, "min_regex_automaton_", regexBaseName);

    cout << "Image generated at: ../../images/min_regex_automaton_" << regexBaseName << ".png\n";

    /*
     * Console reporting of the minimized form
     */
    if (minimized.empty()) {
        cout << "Resulting language is EMPTY (∅)\n";
    } else if (minimized == "#") {
        cout << "Resulting language is { ε }\n";
    } else {
        cout << "Minimized regex: " << minimized << endl;
    }
}
