#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <iostream>
#include <map>
#include <string>

using namespace std;

void checkIsomorphism() {
    string file1;
    string file2;

    cout << "\nEnter the first minimized DFA filename (without .txt): ";
    
    cin >> file1;
    
    cout << "Enter the second minimized DFA filename (without .txt): ";

    cin >> file2;

    string path1 = "../../outputs/" + file1 + ".txt";
    string path2 = "../../outputs/" + file2 + ".txt";

    Automaton A = Automaton::readAutomaton(path1);
    Automaton B = Automaton::readAutomaton(path2);

    map<int, int> mapping;
    
    cout << "\nChecking isomorphism between " << file1 << " and " << file2 << "...\n";

    if (Automaton::isIsomorphic(A, B, &mapping)) {
        cout << "The DFAs are isomorphic (equivalent up to renaming)." << endl;
        
        Dot dotGenerator;

        dotGenerator.generateIsomorphismDot(A, B, mapping, file1 + "_vs_" + file2);
    
        cout << "Visualization generated: MTP/images/iso_" << file1 << "_vs_" << file2 << ".png" << endl;
    } else {
        cout << "The DFAs are NOT isomorphic." << endl;
    }
}