#include "../include/Automaton.h"
#include "../include/Dot.h"
#include "../include/RegexENFA.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

/*
 * ENFAFragment
 *
 * Used in Thompson's construction. Each fragment contains:
 *   start : entry state of the fragment
 *   end   : exit state of the fragment
 *
 * Larger ENFA structures are formed by connecting fragments via
 * ε-transitions and literal transitions.
 */
struct ENFAFragment {
    int start;
    int end;
};

/*
 * Global state counter used to assign unique ENFA states.
 */
static int stateCounter = 0;

/*
 * createBasicENFA(A, symbol)
 *
 * Creates a basic ENFA fragment for a single literal transition:
 *
 *      s1 --symbol--> s2
 *
 * Returns {s1, s2}.
 */
static ENFAFragment createBasicENFA(Automaton& A, char symbol) {
    int s1 = stateCounter++;
    int s2 = stateCounter++;
    A.addState(s1);
    A.addState(s2);
    A.addTransition(s1, symbol, s2);
    return {s1, s2};
}

/*
 * addEpsilonTransition(A, from, to)
 *
 * Adds an ε-transition using '#' to represent epsilon.
 */
static void addEpsilonTransition(Automaton& A, int from, int to) {
    A.addTransition(from, '#', to);
}

/*
 * addConcatenation(regex)
 *
 * Inserts explicit '.' operators between places where concatenation
 * is implied. For example:
 *
 *      a(b|c)*d   →   a.(b|c)*.d
 *
 * Required to simplify operator precedence handling.
 */
static string addConcatenation(const string& regex) {
    string result;

    for (size_t i = 0; i < regex.size(); ++i) {
        char c1 = regex[i];
        result += c1;

        if (i + 1 < regex.size()) {
            char c2 = regex[i + 1];

            bool left  = (isalnum(c1) || c1 == '#' || c1 == '*' || c1 == ')');
            bool right = (isalnum(c2) || c2 == '#' || c2 == '(');

            if (left && right) {
                result += '.';
            }
        }
    }
    return result;
}

/*
 * regexToENFA(inputBaseName)
 *
 * Reads a regular expression from ../../inputs/<inputBaseName>.txt
 * and constructs its ε-NFA (ENFA) using Thompson's construction.
 *
 * Steps:
 *   1. Read regex from file
 *   2. Insert explicit concatenation operators
 *   3. Use two stacks (operators + fragments)
 *   4. Apply precedence rules:  '*' > '.' > '|'
 *   5. Build ENFA via Thompson rules
 *   6. Write ENFA to outputs folder
 *   7. Generate DOT and PNG images
 */
Automaton regexToENFA(const string& inputBaseName) {

    stateCounter = 0;

    string inputPath  = "../../inputs/" + inputBaseName + ".txt";
    string outputPath = "../../outputs/enfa_" + inputBaseName + ".txt";
    string dotPath    = "../../dots/enfa_" + inputBaseName + ".dot";

    /*
     * Step 1: Read regex from file
     */
    ifstream fin(inputPath);
    string regex;
    getline(fin, regex);
    fin.close();

    /*
     * Step 2: Insert explicit concatenation operators
     */
    regex = addConcatenation(regex);

    /*
     * Stacks used by the shunting-yard-like construction:
     *   fragStack : ENFA fragments
     *   opStack   : operators
     */
    Automaton A;
    stack<ENFAFragment> fragStack;
    stack<char> opStack;

    /*
     * Precedence function.
     */
    auto precedence = [](char op) {
        if (op == '*') return 3;
        if (op == '.') return 2;
        if (op == '|') return 1;
        return 0;
    };

    /*
     * applyOp(op)
     *
     * Applies a regex operator to the ENFA fragment stack.
     *
     * Handles:
     *   - Union     : f1 | f2
     *   - Concat    : f1 . f2
     *   - Kleene *  : f*
     *
     * Performs necessary ε-transitions.
     */
    auto applyOp = [&](char op) {
        if (op == '|' || op == '.') {
            if (fragStack.size() < 2) {
                cerr << "[ERROR] Malformed regex: insufficient operands for '" << op << "'\n";
                return;
            }
        } else if (op == '*') {
            if (fragStack.empty()) {
                cerr << "[ERROR] Malformed regex: '*' missing operand\n";
                return;
            }
        }

        if (op == '|') {
            ENFAFragment f2 = fragStack.top(); fragStack.pop();
            ENFAFragment f1 = fragStack.top(); fragStack.pop();

            int start = stateCounter++;
            int end   = stateCounter++;
            A.addState(start);
            A.addState(end);

            addEpsilonTransition(A, start, f1.start);
            addEpsilonTransition(A, start, f2.start);
            addEpsilonTransition(A, f1.end, end);
            addEpsilonTransition(A, f2.end, end);

            fragStack.push({start, end});
        }

        else if (op == '.') {
            ENFAFragment f2 = fragStack.top(); fragStack.pop();
            ENFAFragment f1 = fragStack.top(); fragStack.pop();

            addEpsilonTransition(A, f1.end, f2.start);
            fragStack.push({f1.start, f2.end});
        }

        else if (op == '*') {
            ENFAFragment f = fragStack.top(); fragStack.pop();

            int start = stateCounter++;
            int end   = stateCounter++;
            A.addState(start);
            A.addState(end);

            addEpsilonTransition(A, start, f.start);
            addEpsilonTransition(A, f.end, end);
            addEpsilonTransition(A, start, end);
            addEpsilonTransition(A, f.end, f.start);

            fragStack.push({start, end});
        }
    };

    /*
     * Step 3: Parse regex by scanning characters
     */
    for (size_t i = 0; i < regex.size(); i++) {
        char c = regex[i];

        if (isalnum(c) || c == '#') {
            fragStack.push(createBasicENFA(A, c));
        }
        else if (c == '(') {
            opStack.push(c);
        }
        else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                applyOp(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        }
        else if (c == '*' || c == '|' || c == '.') {
            while (!opStack.empty() &&
                   opStack.top() != '(' &&
                   precedence(opStack.top()) >= precedence(c)) {

                applyOp(opStack.top());
                opStack.pop();
            }
            opStack.push(c);
        }
    }

    /*
     * Apply remaining operators
     */
    while (!opStack.empty()) {
        applyOp(opStack.top());
        opStack.pop();
    }

    /*
     * Combine final fragment into ENFA
     */
    if (fragStack.empty()) {
        cerr << "[ERROR] regexToENFA: No fragments were produced\n";
        return A;
    }

    ENFAFragment result = fragStack.top();
    fragStack.pop();

    A.addInitialState(result.start);
    A.addFinalState(result.end);

    /*
     * Write automaton + generate DOT + PNG
     */
    A.writeAutomaton(outputPath);

    Dot dotGen;
    dotGen.generateDot(outputPath, dotPath, "ENFA");
    dotGen.generateImage(dotPath, "enfa_", inputBaseName);

    return A;
}
