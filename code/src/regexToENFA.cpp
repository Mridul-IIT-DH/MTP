#include "../include/Automaton.h"
#include "../include/Dot.h"
#include "../include/RegexENFA.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

struct ENFAFragment {
    int start;
    int end;
};

static int stateCounter = 0;

static ENFAFragment createBasicENFA(Automaton& A, char symbol) {
    int s1 = stateCounter++;
    int s2 = stateCounter++;
    A.addState(s1);
    A.addState(s2);
    A.addTransition(s1, symbol, s2);
    return {s1, s2};
}

static void addEpsilonTransition(Automaton& A, int from, int to) {
    A.addTransition(from, '#', to);
}

static string addConcatenation(const string& regex) {
    string result;
    for (size_t i = 0; i < regex.size(); ++i) {
        char c1 = regex[i];
        result += c1;

        if (i + 1 < regex.size()) {
            char c2 = regex[i + 1];

            if ((isalnum(c1) || c1 == '*' || c1 == ')') &&
                (isalnum(c2) || c2 == '(')) {
                result += '.';
            }
        }
    }
    return result;
}

Automaton regexToENFA(const string& inputBaseName) {
    stateCounter = 0;

    string inputPath = "../../inputs/" + inputBaseName + ".txt";
    string outputPath = "../../outputs/enfa_" + inputBaseName + ".txt";
    string dotPath = "../../dots/enfa_" + inputBaseName + ".dot";

    ifstream fin(inputPath);
    string regex;
    getline(fin, regex);
    fin.close();

    regex = addConcatenation(regex); // handle implicit concatenation

    Automaton A;
    stack<ENFAFragment> fragStack;
    stack<char> opStack;

    auto applyOp = [&](char op) {
        if (op == '|') {
            ENFAFragment f2 = fragStack.top(); fragStack.pop();
            ENFAFragment f1 = fragStack.top(); fragStack.pop();
            int start = stateCounter++;
            int end = stateCounter++;
            A.addState(start); A.addState(end);
            addEpsilonTransition(A, start, f1.start);
            addEpsilonTransition(A, start, f2.start);
            addEpsilonTransition(A, f1.end, end);
            addEpsilonTransition(A, f2.end, end);
            fragStack.push({start, end});
        } else if (op == '.') {
            ENFAFragment f2 = fragStack.top(); fragStack.pop();
            ENFAFragment f1 = fragStack.top(); fragStack.pop();
            addEpsilonTransition(A, f1.end, f2.start);
            fragStack.push({f1.start, f2.end});
        } else if (op == '*') {
            ENFAFragment f = fragStack.top(); fragStack.pop();
            int start = stateCounter++;
            int end = stateCounter++;
            A.addState(start); A.addState(end);
            addEpsilonTransition(A, start, f.start);
            addEpsilonTransition(A, f.end, end);
            addEpsilonTransition(A, start, end);
            addEpsilonTransition(A, f.end, f.start);
            fragStack.push({start, end});
        }
    };

    auto precedence = [](char op) {
        if (op == '*') return 3;
        if (op == '.') return 2;
        if (op == '|') return 1;
        return 0;
    };

    for (size_t i = 0; i < regex.size(); i++) {
        char c = regex[i];
        if (isalnum(c)) {
            fragStack.push(createBasicENFA(A, c));
        } else if (c == '(') {
            opStack.push(c);
        } else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                applyOp(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        } else if (c == '*' || c == '|' || c == '.') {
            while (!opStack.empty() && opStack.top() != '(' &&
                   precedence(opStack.top()) >= precedence(c)) {
                applyOp(opStack.top());
                opStack.pop();
            }
            opStack.push(c);
        }
    }

    while (!opStack.empty()) {
        applyOp(opStack.top());
        opStack.pop();
    }

    ENFAFragment result = fragStack.top();

    A.addInitialState(result.start);
    A.addFinalState(result.end);

    A.writeAutomaton(outputPath);

    Dot dotGen;
    dotGen.generateDot(outputPath, dotPath, "ENFA");
    dotGen.generateImage(dotPath, "enfa_", inputBaseName);

    return A;
}