#include "../include/Automaton.h"
#include "../include/Dot.h"
#include "../include/RegexENFA.h"

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>

using namespace std;

static set<int> epsilonClosure(const Automaton& A, int state) {
    set<int> closure;
    queue<int> q;
    closure.insert(state);
    q.push(state);

    while (!q.empty()) {
        int s = q.front(); q.pop();
        auto it = A.getTransitions().find({s, '#'});
        if (it != A.getTransitions().end()) {
            for (int t : it->second) {
                if (!closure.count(t)) {
                    closure.insert(t);
                    q.push(t);
                }
            }
        }
    }
    return closure;
}

static set<int> epsilonClosureSet(const Automaton& A, const set<int>& states) {
    set<int> closure;
    for (int s : states) {
        set<int> temp = epsilonClosure(A, s);
        closure.insert(temp.begin(), temp.end());
    }
    return closure;
}

Automaton eNFAtoNFA(const string& inputBaseName) {
    string inputPath = "../../outputs/enfa_" + inputBaseName + ".txt";
    string outputPath = "../../outputs/nfa_" + inputBaseName + ".txt";
    string dotPath = "../../dots/nfa_" + inputBaseName + ".dot";

    Automaton E = Automaton::readAutomaton(inputPath);
    Automaton N;
    N.setAlphabet(E.getAlphabet());

    map<set<int>, int> stateMapping;
    queue<set<int>> q;
    int nextId = 0;

    set<int> startClosure = epsilonClosureSet(E, E.getInitialStates());
    q.push(startClosure);
    stateMapping[startClosure] = nextId++;
    N.addInitialState(0);

    while (!q.empty()) {
        set<int> current = q.front(); q.pop();
        int curId = stateMapping[current];
        N.addState(curId);

        for (int s : current) {
            if (E.getFinalStates().count(s)) {
                N.addFinalState(curId);
                break;
            }
        }

        for (char c : E.getAlphabet()) {
            if (c == '#') continue;

            set<int> nextStates;
            for (int s : current) {
                auto it = E.getTransitions().find({s, c});
                if (it != E.getTransitions().end()) {
                    for (int t : it->second) {
                        set<int> tClosure = epsilonClosure(E, t);
                        nextStates.insert(tClosure.begin(), tClosure.end());
                    }
                }
            }

            if (nextStates.empty()) continue;

            if (!stateMapping.count(nextStates)) {
                stateMapping[nextStates] = nextId++;
                q.push(nextStates);
            }

            N.addTransition(curId, c, stateMapping[nextStates]);
        }
    }

    N.writeAutomaton(outputPath);

    Dot dotGen;
    dotGen.generateDot(outputPath, dotPath, "NFA");
    dotGen.generateImage(dotPath, "nfa_", inputBaseName);

    return N;
}