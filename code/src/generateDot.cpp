#include "../include/Dot.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

void Dot::generateDot(const string& inputFile, const string& outputFile, const string& graphName) {
    string line;

    ifstream fin(inputFile);

    vector<tuple<int, char, int>> transitions;
    vector<int> initialStates;
    vector<int> finalStates;

    while(getline(fin, line)) {
        if (line == "TRANSITIONS:") {
            while (getline(fin, line) && !line.empty()) {
                stringstream ss(line);

                int from;
                char c;
                int to;

                ss >> from >> c >> to;

                transitions.push_back({from, c, to});
            }
        } else if (line == "INITIAL_STATES:") {
            if (getline(fin, line)) {
                stringstream ss(line);
                
                int temp;
    
                while (ss >> temp) {
                    initialStates.push_back(temp);
                }
            }
        } else if (line == "FINAL_STATES:") {
            if (getline(fin, line)) {
                stringstream ss(line);
                
                int temp;
    
                while (ss >> temp) {
                    finalStates.push_back(temp);
                }
            }
        }
    }

    ofstream fout(outputFile);

    fout << "digraph " << graphName << "{" 
                       << "\n\t"
                       << "rankdir=LR;"
                       << "\n\t"
                       << "size=\"8,5\";"
                       << "\n\n\t"
                       << "node [shape = doublecircle];"
                       << "\n\t";
                       
                       for (int finalState : finalStates) {
                            fout << finalState << " ";
                       }

                fout   << ";"
                       << "\n\t"
                       << "node [shape = circle];"
                       << "\n\t"
                       << "start [shape=point];"
                       << "\n\t";
                       
                       for (int initialState : initialStates) {
                            fout << "start -> " << initialState << ";\n";
                       }

                fout   << "\n\t";

    for (auto& transition : transitions) {
        fout
            << get<0>(transition) 
            << " -> "
            << get<2>(transition)
            << "[label=\""
            << get<1>(transition) 
            << "\"];"
            << "\n\t";
    }

    fout << "\n}";
}