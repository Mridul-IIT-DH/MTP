#include "../include/Automaton.h"

#include <map>
#include <queue>
#include <set>

using namespace std;

bool Automaton::isIsomorphic(const Automaton& A, const Automaton& B, map<int, int>* mappingOut) {
    if (A.getAlphabet() != B.getAlphabet()) {
        return false;
    }

    if (A.getInitialStates().size() != 1 ||
        B.getInitialStates().size() != 1) {
        return false;
    }

    int startA = *A.getInitialStates().begin();
    int startB = *B.getInitialStates().begin();

    map<int, int> mappingAB;
    map<int, int> mappingBA;

    queue<pair<int, int>> q;
    
    q.push({startA, startB});

    mappingAB[startA] = startB;
    mappingBA[startB] = startA;

    while (!q.empty()) {
        auto [a, b] = q.front();
        q.pop();

        bool aFinal = A.getFinalStates().count(a);
        bool bFinal = B.getFinalStates().count(b);

        if (aFinal != bFinal) {
            return false;
        }

        for (char c : A.getAlphabet()) {
            const auto& Atrans = A.getTransitions();
            const auto& Btrans = B.getTransitions();

            auto itA = Atrans.find({a, c});
            auto itB = Btrans.find({b, c});

            if (itA == Atrans.end() || itB == Btrans.end()) {
                return false;
            }

            int toA = *itA->second.begin();
            int toB = *itB->second.begin();

            if (!mappingAB.count(toA) && !mappingBA.count(toB)) {
                mappingAB[toA] = toB;
                mappingBA[toB] = toA;

                q.push({toA, toB});
            } else if (mappingAB.count(toA) && mappingAB[toA] != toB) {
                return false;
            }
        }
    }

    if (mappingAB.size() != A.getStates().size() ||
        mappingBA.size() != B.getStates().size()) {
        return false;
    }

    if (mappingOut != nullptr) {
        *mappingOut = mappingAB;
    }

    return true;
}