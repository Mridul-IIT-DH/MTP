#include "../include/Automaton.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Automaton Automaton::readAutomaton(const string& filename) {
    fstream fin(filename);

    Automaton A;

    string line;
    string label;

    while (getline(fin, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        ss >> label;

        if (label == "STATES:") {
            int s;

            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);

                while (ss2 >> s) {
                    A.states.insert(s);
                }
            }
        } else if (label == "ALPHABET:") {
            char c;

            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);

                while (ss2 >> c) {
                    A.alphabet.insert(c);
                }
            }
        } else if (label == "TRANSITIONS:") {
            int from;
            int to;
            char c;

            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);

                ss2 >> from >> c >> to;

                A.transitions[{from, c}].insert(to);
            }
        } else if (label == "INITIAL_STATES:") {
            int s;

            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);

                while (ss2 >> s) {
                    A.initialStates.insert(s);
                }
            }
        } else if (label == "FINAL_STATES:") {
            int s;

            while (getline(fin, line) && !line.empty()) {
                stringstream ss2(line);

                while (ss2 >> s) {
                    A.finalStates.insert(s);
                }
            }
        }
    }

    fin.close();

    return A;
}