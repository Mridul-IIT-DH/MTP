#include "../include/Automaton.h"

#include <fstream>

void Automaton::writeAutomaton(const std::string &filename) const {
    std::ofstream fout(filename);

    fout << "STATES:\n";

    for (int s : states) {
        fout << s << " ";
    }

    fout << "\n\n";

    fout << "ALPHABET:\n";

    for (char c : alphabet) {
        fout << c << " ";
    }

    fout << "\n\n";

    fout << "TRANSITIONS:\n";

    for (auto &p : transitions) {
        for (int to : p.second) {
            fout << p.first.first << " " << p.first.second << " " << to << "\n";
        }
    }

    fout << "\n";

    fout << "INITIAL_STATES:\n";

    for (int s : initialStates) {
        fout << s << " ";
    }

    fout << "\n\n";

    fout << "FINAL_STATES:\n";

    for (int s : finalStates) {
        fout << s << " ";
    }

    fout << "\n";

    fout.close();
}