#include "../include/NFAToRegex.h"
#include "../include/RegexParser.h"
#include "../include/RegexNormalize.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <climits>

using namespace std;

/*
 * regexUnion(r1, r2)
 *
 * Returns the union of two regex strings.
 * Handles empty cases, duplicate avoidance, and alphabetical ordering.
 */
static string regexUnion(string r1, string r2) {
    if (r1.empty()) return r2;
    if (r2.empty()) return r1;
    if (r1 == r2) return r1;
    if (r1 > r2) swap(r1, r2);
    return "(" + r1 + "|" + r2 + ")";
}

/*
 * regexConcat(r1, r2)
 *
 * Concatenation of two regex strings.
 * Removes epsilon (#) where appropriate and adds parentheses
 * when operands contain union operations.
 */
static string regexConcat(string r1, string r2) {
    if (r1.empty() || r2.empty()) return "";
    if (r1 == "#") return r2;
    if (r2 == "#") return r1;

    if (r1.find('|') != string::npos && r1.front() != '(')
        r1 = "(" + r1 + ")";
    if (r2.find('|') != string::npos && r2.front() != '(')
        r2 = "(" + r2 + ")";

    return r1 + r2;
}

/*
 * regexStar(r)
 *
 * Applies the Kleene star to r.
 * Takes care of epsilon (#), empty, and redundant star forms.
 */
static string regexStar(string r) {
    if (r.empty()) return "#";
    if (r == "#") return "#";
    if (r.size() == 1) return r + "*";
    if (r.back() == '*') return r;
    return "(" + r + ")*";
}

/*
 * automatonToRegex(A)
 *
 * Converts a (possibly ε-)NFA into a regular expression using the
 * state elimination method. Steps:
 *
 *   1. Copy automaton A into P.
 *   2. Add a new global start and final state with ε transitions.
 *   3. Initialize R[u,v] to store regex labels for edges.
 *   4. Eliminate intermediate states using GNFA update rules:
 *        R[i,j] = R[i,j] ∪ ( R[i,k] (R[k,k])* R[k,j] )
 *   5. When only start and final remain, return R[start,final].
 *   6. Pass through AST normalizer for readability.
 */
string automatonToRegex(const Automaton& A) {

    Automaton P;

    /*
     * Copy states and transitions from A into P.
     */
    for (int s : A.getStates()) P.addState(s);

    for (auto const& [key, to_set] : A.getTransitions()) {
        int u = key.first;
        char sym = key.second;
        for (int v : to_set) P.addTransition(u, sym, v);
    }

    P.setAlphabet(A.getAlphabet());

    /*
     * Add new GNFA (Generalized Nondeterministic Finite Automaton) start and final states.
     */
    int maxState = 0;
    for (int s : P.getStates()) maxState = max(maxState, s);

    int newStart = maxState + 1;
    int newFinal = maxState + 2;

    P.addState(newStart);
    P.addState(newFinal);

    /*
     * ε-transitions from new start to original start states,
     * and from original final states to new final.
     */
    for (int s : A.getInitialStates()) {
        P.addTransition(newStart, '#', s);
    }

    for (int s : A.getFinalStates()) {
        P.addTransition(s, '#', newFinal);
    }

    P.addInitialState(newStart);
    P.addFinalState(newFinal);

    /*
     * Initialize R[u,v] table mapping state pairs to regex strings.
     */
    map<pair<int,int>, string> R;
    set<int> allStates = P.getStates();
    set<int> statesToEliminate;

    for (int s : allStates) {
        if (s != newStart && s != newFinal)
            statesToEliminate.insert(s);
    }

    for (auto const& [key, to_set] : P.getTransitions()) {
        int u = key.first;
        char sym = key.second;
        string t(1, sym);

        for (int v : to_set) {
            R[{u,v}] = regexUnion(R[{u,v}], t);
        }
    }

    /*
     * State elimination loop.
     * Select next state to eliminate using the heuristic
     *   score = indegree * outdegree + indegree + outdegree
     */
    while (!statesToEliminate.empty()) {

        int bestState = -1;
        int bestScore = INT_MAX;

        for (int k : statesToEliminate) {
            int indeg = 0, outdeg = 0;

            for (auto &p : R) {
                if (!p.second.empty() && p.first.second == k) indeg++;
                if (!p.second.empty() && p.first.first == k) outdeg++;
            }

            int score = indeg * outdeg + indeg + outdeg;
            if (score < bestScore) {
                bestScore = score;
                bestState = k;
            }
        }

        int q_rip = bestState;
        statesToEliminate.erase(q_rip);

        /*
         * Compute R[k,k]* for self-loops.
         */
        string R_kk = regexStar(R[{q_rip, q_rip}]);

        /*
         * Update all pairs (i, j) using GNFA combination rule.
         */
        for (int i : allStates) {
            if (i == q_rip) continue;

            string R_ik = R[{i, q_rip}];
            if (R_ik.empty()) continue;

            for (int j : allStates) {
                if (j == q_rip) continue;

                string R_kj = R[{q_rip, j}];
                if (R_kj.empty()) continue;

                string R_old = R[{i,j}];
                string R_new = regexConcat(regexConcat(R_ik, R_kk), R_kj);

                R[{i,j}] = regexUnion(R_old, R_new);
            }
        }

        /*
         * Remove all transitions involving k.
         */
        map<pair<int,int>, string> temp;
        for (auto &p : R) {
            if (p.first.first != q_rip && p.first.second != q_rip)
                temp[p.first] = p.second;
        }
        R = std::move(temp);
    }

    /*
     * Final regex is the expression from newStart to newFinal.
     */
    string raw = R[{newStart, newFinal}];

    if (raw.empty()) return "";

    /*
     * Parse and prettify the regex using AST normalizer.
     */
    auto ast = parseRegexToAST(raw);
    ast = prettifyRegexAST(ast);

    return ast->toString();
}
