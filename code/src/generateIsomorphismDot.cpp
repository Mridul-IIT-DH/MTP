#include "../include/Automaton.h"

#include <fstream>
#include <map>
#include <string>
#include <iostream>

using namespace std;

void generateIsomorphismDot(
    const Automaton& A,
    const Automaton& B,
    const map<int, int>& mapping,
    const string& filenameBase
) {
    string dotFile = "../../dots/iso_" + filenameBase + ".dot";
    string pngFile = "../../images/iso_" + filenameBase + ".png";

    ofstream fout(dotFile);
    if (!fout.is_open()) {
        cerr << "Error: Unable to open " << dotFile << " for writing.\n";
        return;
    }

    fout << "digraph Isomorphism {";
    fout << "\n\t";
    fout << "rankdir=LR;";
    fout << "\n\t";
    fout << "node [shape=circle, style=filled, fillcolor=white];";
    fout << "\n\t";
    fout << "splines=true;";

    fout << "\n\t";
    fout << "subgraph cluster_A {";
    fout << "\n\t\t";
    fout << "label=\"Automaton A\";";
    fout << "\n\t\t";
    fout << "color=lightgrey;";
    for (int s : A.getStates()) {
        bool isFinal = A.getFinalStates().count(s);
        string color = isFinal ? "palegreen" : "white";
        fout << "\n\t\t";
        fout << "A" << s << " [label=\"A" << s << "\", fillcolor=\"" << color << "\"];";
    }
    fout << "\n\t";
    fout << "}";

    fout << "\n\t";
    fout << "subgraph cluster_B {";
    fout << "\n\t\t";
    fout << "label=\"Automaton B\";";
    fout << "\n\t\t";
    fout << "color=lightgrey;";
    for (int s : B.getStates()) {
        bool isFinal = B.getFinalStates().count(s);
        string color = isFinal ? "palegreen" : "white";
        fout << "\n\t\t";
        fout << "B" << s << " [label=\"B" << s << "\", fillcolor=\"" << color << "\"];";
    }
    fout << "\n\t";
    fout << "}";

    fout << "\n\t";

    for (int s : A.getInitialStates()) {
        fout << "\n\t";
        fout << "initA" << s << " [shape=point, style=invis];";
        fout << "\n\t";
        fout << "initA" << s << " -> A" << s << " [label=\"\", color=black];";
    }
    for (int s : B.getInitialStates()) {
        fout << "\n\t";
        fout << "initB" << s << " [shape=point, style=invis];";
        fout << "\n\t";
        fout << "initB" << s << " -> B" << s << " [label=\"\", color=black];";
    }

    fout << "\n\t";

    for (auto& [a, b] : mapping) {
        fout << "\n\t";
        fout << "A" << a << " -> B" << b << " [color=blue, style=dashed, label=\"≈\"];";
    }

    fout << "\n";
    fout << "}";

    fout.close();

    string command = "dot -Tpng \"" + dotFile + "\" -o \"" + pngFile + "\"";

    system(command.c_str());
}