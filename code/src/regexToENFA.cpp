#include "../include/Automaton.h"
#include "../include/Dot.h"
#include "../include/RegexENFA.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

/**
 * @brief A simple structure representing a partially-built ENFA fragment.
 *
 * Each fragment has:
 *   - start : entry state of the fragment
 *   - end   : exit (accepting) state of the fragment
 *
 * Thompson’s construction connects these fragments together when
 * applying operations such as concatenation, union, or Kleene star.
 */
struct ENFAFragment {
    int start;
    int end;
};

// Global counter for generating unique state IDs
static int stateCounter = 0;

/* ================================================================
   Thompson Construction — Basic Building Blocks
   ================================================================ */

/**
 * @brief Creates a basic ENFA fragment representing a single symbol transition.
 *
 * For symbol 'a', this creates:
 *
 *        (s1) --a--> (s2)
 *
 * @param A       Automaton to which states and transitions are added.
 * @param symbol  Input character for the transition.
 * @return ENFAFragment {start → end}
 */
static ENFAFragment createBasicENFA(Automaton& A, char symbol) {
    int s1 = stateCounter++;
    int s2 = stateCounter++;
    A.addState(s1);
    A.addState(s2);
    A.addTransition(s1, symbol, s2);
    return {s1, s2};
}

/**
 * @brief Adds an ε-transition (represented by '#') to the automaton.
 */
static void addEpsilonTransition(Automaton& A, int from, int to) {
    A.addTransition(from, '#', to);
}

/* ================================================================
   Utility: Insert explicit concatenation operator '.'
   ================================================================ */

/**
 * @brief Converts an implicit-concatenation regex into one containing
 *        explicit '.' operators.
 *
 * Example:
 *   "ab(c|d)*e"  →  "a.b.(c|d)*.e"
 *
 * This simplifies parsing using operator-precedence rules.
 */
static string addConcatenation(const string& regex) {
    string result;

    for (size_t i = 0; i < regex.size(); ++i) {
        char c1 = regex[i];
        result += c1;

        if (i + 1 < regex.size()) {
            char c2 = regex[i + 1];

            // Insert '.' (concatenation) when:
            //   - a literal or ')' or '*' is followed by
            //   - a literal or '('
            if ((isalnum(c1) || c1 == '*' || c1 == ')') &&
                (isalnum(c2) || c2 == '(')) {
                result += '.';
            }
        }
    }

    return result;
}

/* ================================================================
   Main Conversion Function — Thompson ENFA Construction
   ================================================================ */

/**
 * @brief Converts a regular expression into an ε-NFA using Thompson’s algorithm.
 *
 * @details
 * Pipeline:
 *   1. Read regex from "../../inputs/<name>.txt"
 *   2. Insert explicit concatenation operators.
 *   3. Use two stacks:
 *        - fragStack : holds ENFA fragments
 *        - opStack   : holds operators ('|', '.', '*', '(')
 *   4. Apply operators using precedence rules:
 *        precedence('*') = 3
 *        precedence('.') = 2
 *        precedence('|') = 1
 *   5. Thompson constructions:
 *
 *      UNION:
 *           start → f1.start
 *           start → f2.start
 *           f1.end → end
 *           f2.end → end
 *
 *      CONCATENATION:
 *           f1.end → f2.start
 *
 *      KLEENE STAR:
 *           start → f.start
 *           start → end
 *           f.end → f.start
 *           f.end → end
 *
 *   6. Final fragment’s start = automaton initial state
 *   7. Final fragment’s end   = automaton final state
 *
 * The constructed ENFA is then written as text, a DOT graph, and a PNG image.
 *
 * @param inputBaseName The name of the regex input file (without .txt extension).
 * @return The constructed ε-NFA.
 */
Automaton regexToENFA(const string& inputBaseName) {

    // Reset state counter for fresh ENFA construction
    stateCounter = 0;

    // --------------------------------------------------------------
    // Step 1: Read regular expression from input file
    // --------------------------------------------------------------
    string inputPath = "../../inputs/" + inputBaseName + ".txt";
    string outputPath = "../../outputs/enfa_" + inputBaseName + ".txt";
    string dotPath    = "../../dots/enfa_"  + inputBaseName + ".dot";

    ifstream fin(inputPath);
    string regex;
    getline(fin, regex);
    fin.close();

    // --------------------------------------------------------------
    // Step 2: Insert explicit concatenation operators
    // --------------------------------------------------------------
    regex = addConcatenation(regex);

    // Storage for fragments and operators
    Automaton A;
    stack<ENFAFragment> fragStack;
    stack<char> opStack;

    /* ===============================================================
       Lambda: apply an operator using Thompson construction rules
       =============================================================== */
    auto applyOp = [&](char op) {
        if (op == '|') {
            // UNION: f1 ∪ f2
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
            // CONCATENATION: f1 ∘ f2
            ENFAFragment f2 = fragStack.top(); fragStack.pop();
            ENFAFragment f1 = fragStack.top(); fragStack.pop();

            addEpsilonTransition(A, f1.end, f2.start);
            fragStack.push({f1.start, f2.end});
        }

        else if (op == '*') {
            // KLEENE STAR: f*
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

    /* ===============================================================
       Precedence rules for operators
       =============================================================== */
    auto precedence = [](char op) {
        if (op == '*') return 3;
        if (op == '.') return 2;
        if (op == '|') return 1;
        return 0;
    };

    /* ===============================================================
       Step 3: Parse regex using stacks (shunting-yard-like)
       =============================================================== */
    for (size_t i = 0; i < regex.size(); i++) {
        char c = regex[i];

        if (isalnum(c)) {
            // Literal → basic ENFA fragment
            fragStack.push(createBasicENFA(A, c));
        }
        else if (c == '(') {
            opStack.push(c);
        }
        else if (c == ')') {
            // Apply operators until '(' is reached
            while (!opStack.empty() && opStack.top() != '(') {
                applyOp(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop(); // remove '('
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

    // Apply remaining operators
    while (!opStack.empty()) {
        applyOp(opStack.top());
        opStack.pop();
    }

    /* ===============================================================
       Step 4: Finalize the ENFA
       =============================================================== */
    ENFAFragment result = fragStack.top();

    A.addInitialState(result.start);
    A.addFinalState(result.end);

    /* ===============================================================
       Step 5: Write ENFA to file + generate DOT + PNG
       =============================================================== */
    A.writeAutomaton(outputPath);

    Dot dotGen;
    dotGen.generateDot(outputPath, dotPath, "ENFA");
    dotGen.generateImage(dotPath, "enfa_", inputBaseName);

    return A;
}
