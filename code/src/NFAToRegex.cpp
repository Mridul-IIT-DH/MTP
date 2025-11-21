#include "../include/NFAToRegex.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

/* ===============================================================
   Helper functions for constructing regular expressions
   =============================================================== */

/**
 * @brief Returns the union (alternation) of two regexes: r1 | r2.
 *
 * Rules:
 *  - If one is empty, return the other.
 *  - If they are equal, return one occurrence.
 *  - Maintain deterministic ordering (lexicographic) for consistency.
 */
static string regexUnion(string r1, string r2) {
    if (r1.empty()) return r2;
    if (r2.empty()) return r1;
    if (r1 == r2) return r1;
    if (r1 > r2) swap(r1, r2);  // deterministic ordering
    return "(" + r1 + "|" + r2 + ")";
}

/**
 * @brief Concatenation of two regexes: r1 r2.
 *
 * Special rules:
 *  - If either is empty → concatenation impossible → return empty.
 *  - '#' (epsilon) acts as identity element.
 *  - Parenthesize expressions containing '|' when needed.
 */
static string regexConcat(string r1, string r2) {
    if (r1.empty() || r2.empty()) return ""; 
    if (r1 == "#") return r2;
    if (r2 == "#") return r1;

    // Add parentheses around union expressions
    if (r1.find('|') != string::npos && r1.front() != '(')
        r1 = "(" + r1 + ")";
    if (r2.find('|') != string::npos && r2.front() != '(')
        r2 = "(" + r2 + ")";

    return r1 + r2;
}

/**
 * @brief Kleene star of a regex: r*.
 *
 * Rules:
 *  - Empty → ε
 *  - ε* = ε
 *  - Avoid unnecessary parentheses where possible.
 */
static string regexStar(string r) {
    if (r.empty()) return "#";
    if (r == "#") return "#";
    if (r.length() == 1) return r + "*";
    if (r.back() == '*') return r;  // avoid (r*)*
    return "(" + r + ")*";
}


/* ===============================================================
   Main NFA-to-Regex Conversion (State Elimination)
   =============================================================== */

/**
 * @brief Converts a (possibly ε-)NFA into a regular expression
 *        using the classical **state elimination algorithm**.
 *
 * ----------------------------------------------------------------
 * THEORETICAL OVERVIEW
 * ----------------------------------------------------------------
 * Given an NFA  A = (Q, Σ, δ, I, F), the algorithm:
 *
 *   1. Creates a new unique start state s and final state f.
 *   2. Adds ε-transitions:
 *         s → i  for each original initial state i
 *         a → f  for each original final state a
 *
 *   3. Constructs a table R[u, v] containing regexes describing
 *      all direct transitions u → v.
 *
 *   4. Eliminates states one by one:
 *        If eliminating k,
 *        update each pair (i, j) by adding:
 *
 *           R[i,j] = R[i,j] ∪ ( R[i,k] · R[k,k]* · R[k,j] )
 *
 *   5. Remove all transitions involving k.
 *
 *   6. When only newStart and newFinal remain:
 *
 *         The resulting regex is R[newStart, newFinal].
 *
 * ----------------------------------------------------------------
 * This algorithm is correct and complete:
 *      L(regex) = L(NFA)
 * ----------------------------------------------------------------
 *
 * @param A   Input automaton.
 * @return    A regular expression describing L(A).
 */
string automatonToRegex(const Automaton& A) {

    /* ==========================================================
       Step 1: Make a working copy P of the automaton A
       ========================================================== */
    Automaton P;

    // Copy states
    for (int s : A.getStates()) {
        P.addState(s);
    }

    // Copy transitions
    for (auto const& [key, to_set] : A.getTransitions()) {
        int u = key.first;
        char symbol = key.second;
        for (int v : to_set) {
            P.addTransition(u, symbol, v);
        }
    }

    // Copy alphabet
    P.setAlphabet(A.getAlphabet());

    /* ==========================================================
       Step 2: Add new start state (s) and new final state (f)
       ========================================================== */

    int maxState = 0;
    for (int s : P.getStates())
        maxState = max(maxState, s);

    int newStart = maxState + 1;
    int newFinal = maxState + 2;

    P.addState(newStart);
    P.addState(newFinal);

    // ε-transitions from newStart to original start states
    for (int s : A.getInitialStates()) {
        P.addTransition(newStart, '#', s);
    }
    
    // ε-transitions from original final states to newFinal
    for (int s : A.getFinalStates()) {
        P.addTransition(s, '#', newFinal);
    }

    // Set new initial/final states
    P.addInitialState(newStart);
    P.addFinalState(newFinal);

    /* ==========================================================
       Step 3: Initialize R[u,v] with one-character regex transitions
       ========================================================== */
    map<pair<int, int>, string> R;
    set<int> allStates = P.getStates();
    set<int> statesToEliminate;

    // We will eliminate all states except newStart and newFinal
    for (int s : allStates) {
        if (s != newStart && s != newFinal)
            statesToEliminate.insert(s);
    }

    // Convert each transition into its regex form
    for (auto const& [key, to_set] : P.getTransitions()) {
        int u = key.first;
        char symbol = key.second;
        string s_symbol(1, symbol);  // turn char into string

        for (int v : to_set) {
            R[{u, v}] = regexUnion(R[{u, v}], s_symbol);
        }
    }


    /* ==========================================================
       Step 4: Eliminate states one by one
       ========================================================== */
    for (int q_rip : statesToEliminate) {

        // Self-loop regex: R[k,k]* (if exists)
        string R_kk = regexStar(R[{q_rip, q_rip}]);

        // For every pair of states (i, j)
        for (int q_i : allStates) {
            if (q_i == q_rip) continue;
            string R_ik = R[{q_i, q_rip}];
            if (R_ik.empty()) continue;

            for (int q_j : allStates) {
                if (q_j == q_rip) continue;
                string R_kj = R[{q_rip, q_j}];
                if (R_kj.empty()) continue;

                // Old R[i,j] and new contribution via k
                string R_old = R[{q_i, q_j}];
                string R_new = regexConcat(regexConcat(R_ik, R_kk), R_kj);

                R[{q_i, q_j}] = regexUnion(R_old, R_new);
            }
        }

        // Remove all R[*][k] and R[k][*]
        map<pair<int, int>, string> temp_R;
        for (auto const& [key, val] : R) {
            if (key.first != q_rip && key.second != q_rip)
                temp_R[key] = val;
        }
        R = temp_R;
    }

    /* ==========================================================
       Step 5: When only newStart and newFinal remain,
               return the regex R[s,f]
       ========================================================== */

    return R[{newStart, newFinal}];
}
