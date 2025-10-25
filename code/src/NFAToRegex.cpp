#include "../include/NFAToRegex.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

static string regexUnion(string r1, string r2) {
    if (r1.empty()) return r2;
    if (r2.empty()) return r1;
    if (r1 == r2) return r1;
    if (r1 > r2) swap(r1, r2);
    return "(" + r1 + "|" + r2 + ")";
}

static string regexConcat(string r1, string r2) {
    if (r1.empty() || r2.empty()) return ""; 
    if (r1 == "#") return r2;
    if (r2 == "#") return r1;

    if (r1.find('|') != string::npos && r1.front() != '(') {
        r1 = "(" + r1 + ")";
    }

    if (r2.find('|') != string::npos && r2.front() != '(') {
        r2 = "(" + r2 + ")";
    }
    
    return r1 + r2;
}

static string regexStar(string r) {
    if (r.empty()) return "#";
    if (r == "#") return "#";
    if (r.length() == 1) return r + "*";
    if (r.back() == '*') return r;
    
    return "(" + r + ")*";
}

string automatonToRegex(const Automaton& A) {

    Automaton P;

    for (int s : A.getStates()) {
        P.addState(s);
    }

    for (auto const& [key, to_set] : A.getTransitions()) {
        int u = key.first;
        char symbol = key.second;
        for (int v : to_set) {
            P.addTransition(u, symbol, v);
        }
    }

    P.setAlphabet(A.getAlphabet());

    int maxState = 0;
    for (int s : P.getStates()) {
        if (s > maxState) maxState = s;
    }
    int newStart = maxState + 1;
    int newFinal = maxState + 2;

    P.addState(newStart);
    P.addState(newFinal);

    for (int s : A.getInitialStates()) {
        P.addTransition(newStart, '#', s);
    }
    
    for (int s : A.getFinalStates()) {
        P.addTransition(s, '#', newFinal);
    }

    P.addInitialState(newStart);
    P.addFinalState(newFinal);

    map<pair<int, int>, string> R;
    set<int> statesToEliminate;
    set<int> allStates = P.getStates();

    for (int s : allStates) {
        if (s != newStart && s != newFinal) {
            statesToEliminate.insert(s);
        }
    }

    for (auto const& [key, to_set] : P.getTransitions()) {
        int u = key.first;
        char symbol = key.second;
        string s_symbol(1, symbol);

        for (int v : to_set) {
            R[{u, v}] = regexUnion(R[{u, v}], s_symbol);
        }
    }

    for (int q_rip : statesToEliminate) {

        string R_kk = regexStar(R[{q_rip, q_rip}]);

        for (int q_i : allStates) {
            if (q_i == q_rip) continue;
            string R_ik = R[{q_i, q_rip}];
            if (R_ik.empty()) continue;

            for (int q_j : allStates) {
                if (q_j == q_rip) continue;
                string R_kj = R[{q_rip, q_j}];
                if (R_kj.empty()) continue;
                
                string R_ij_old = R[{q_i, q_j}];
                string R_new = regexConcat(regexConcat(R_ik, R_kk), R_kj);
                
                R[{q_i, q_j}] = regexUnion(R_ij_old, R_new);
            }
        }
        
        map<pair<int, int>, string> temp_R;
        for(auto const& [key, val] : R) {
            if (key.first != q_rip && key.second != q_rip) {
                temp_R[key] = val;
            }
        }
        R = temp_R;
    }

    return R[{newStart, newFinal}];
}