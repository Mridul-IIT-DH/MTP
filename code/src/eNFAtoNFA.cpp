#include "../include/Automaton.h"
#include "../include/Dot.h"
#include "../include/RegexENFA.h"

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>

using namespace std;

/**
 * @brief Computes the ε-closure (epsilon-closure) of a single state in an automaton.
 *
 * @details
 * The ε-closure of a state `q` in an ε-NFA `A = (Q, Σ ∪ {ε}, δ, I, F)` is defined as:
 *   ε-closure(q) = { q' ∈ Q | q →* q' via ε-transitions only }
 *
 * That is, all states reachable from `q` by taking zero or more ε-transitions (`#` here represents ε).
 *
 * @param A       The ε-NFA automaton.
 * @param state   The starting state whose ε-closure is to be computed.
 * @return        A set of states reachable from `state` by only ε-transitions.
 */
static set<int> epsilonClosure(const Automaton& A, int state) {
    set<int> closure;
    queue<int> q;

    // Initialize closure with the starting state itself
    closure.insert(state);
    q.push(state);

    // Standard BFS over ε-transitions
    while (!q.empty()) {
        int s = q.front(); 
        q.pop();

        // Look for ε-transitions from this state ('#' denotes ε)
        auto it = A.getTransitions().find({s, '#'});
        if (it != A.getTransitions().end()) {
            for (int t : it->second) {
                // If we haven’t already added this state, add it and continue exploring
                if (!closure.count(t)) {
                    closure.insert(t);
                    q.push(t);
                }
            }
        }
    }

    return closure;
}

/**
 * @brief Computes the ε-closure of a *set* of states.
 *
 * This is simply the union of the ε-closures of all states in the given set.
 *
 * @param A       The ε-NFA automaton.
 * @param states  A set of states.
 * @return        A set containing all states reachable by ε-transitions from any state in `states`.
 */
static set<int> epsilonClosureSet(const Automaton& A, const set<int>& states) {
    set<int> closure;
    for (int s : states) {
        set<int> temp = epsilonClosure(A, s);
        closure.insert(temp.begin(), temp.end());
    }
    return closure;
}

/**
 * @brief Converts an ε-NFA (E) into an equivalent NFA (N) without ε-transitions.
 *
 * @details
 * ### Theoretical Background:
 * Given an ε-NFA `E = (Q_E, Σ ∪ {ε}, δ_E, I_E, F_E)`, the equivalent NFA `N`
 * is constructed as follows:
 *
 * 1. The start state(s) of `N` is ε-closure(I_E) — all states reachable by ε-transitions
 *    from any initial state.
 * 2. A state `S ⊆ Q_E` in `N` is *final* if `S ∩ F_E ≠ ∅` (any contained E-final state).
 * 3. For every symbol `a ∈ Σ` and state `q ∈ S`:
 *      δ_N(S, a) = ε-closure(⋃_{q ∈ S} δ_E(q, a))
 *
 * The resulting automaton `N` recognizes the same language:
 *     L(N) = L(E)
 *
 * ### Implementation Details:
 * - The algorithm performs a BFS over subsets of ε-closures.
 * - Each unique set of ε-reachable states is represented as a distinct NFA state.
 * - The transition symbol '#' is treated as ε.
 *
 * @param inputBaseName  Base name of the input ε-NFA file (without extension).
 *                       Reads from "../../outputs/enfa_<inputBaseName>.txt".
 * @return               An Automaton object representing the equivalent NFA.
 */
Automaton eNFAtoNFA(const string& inputBaseName) {
    // Step 1: Construct file paths for input and outputs.
    string inputPath  = "../../outputs/enfa_" + inputBaseName + ".txt";
    string outputPath = "../../outputs/nfa_"  + inputBaseName + ".txt";
    string dotPath    = "../../dots/nfa_"     + inputBaseName + ".dot";

    // Step 2: Read the ε-NFA from file.
    Automaton E = Automaton::readAutomaton(inputPath);

    // Step 3: Create a new Automaton N (the resulting NFA).
    Automaton N;
    N.setAlphabet(E.getAlphabet());  // Copy alphabet (excluding ε later)

    // Step 4: Maps subsets of ε-NFA states → unique NFA state IDs.
    map<set<int>, int> stateMapping;
    queue<set<int>> q;
    int nextId = 0;

    // Step 5: Compute ε-closure of the ε-NFA’s initial states → new start state.
    set<int> startClosure = epsilonClosureSet(E, E.getInitialStates());
    q.push(startClosure);
    stateMapping[startClosure] = nextId++;
    N.addInitialState(0);

    // Step 6: BFS over all reachable subsets of states.
    while (!q.empty()) {
        set<int> current = q.front(); 
        q.pop();

        int curId = stateMapping[current];
        N.addState(curId);

        // Mark as final if any original ε-NFA final state is contained in this set.
        for (int s : current) {
            if (E.getFinalStates().count(s)) {
                N.addFinalState(curId);
                break;
            }
        }

        // Step 7: Process transitions for all non-ε symbols.
        for (char c : E.getAlphabet()) {
            if (c == '#') continue;  // Skip ε

            set<int> nextStates;

            // For each state in the current set, collect transitions on symbol c.
            for (int s : current) {
                auto it = E.getTransitions().find({s, c});
                if (it != E.getTransitions().end()) {
                    for (int t : it->second) {
                        // Apply ε-closure to each target state
                        set<int> tClosure = epsilonClosure(E, t);
                        nextStates.insert(tClosure.begin(), tClosure.end());
                    }
                }
            }

            if (nextStates.empty()) continue;

            // If we discover a new subset, assign it a new state ID.
            if (!stateMapping.count(nextStates)) {
                stateMapping[nextStates] = nextId++;
                q.push(nextStates);
            }

            // Add the resulting transition to the new NFA.
            N.addTransition(curId, c, stateMapping[nextStates]);
        }
    }

    // Step 8: Write the resulting NFA to file.
    N.writeAutomaton(outputPath);

    // Step 9: Generate visualization for the new NFA.
    Dot dotGen;
    dotGen.generateDot(outputPath, dotPath, "NFA");
    dotGen.generateImage(dotPath, "nfa_", inputBaseName);

    // Step 10: Return the constructed NFA.
    return N;
}
