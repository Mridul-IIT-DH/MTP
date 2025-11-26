#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <map>
#include <set>
#include <string>
#include <vector>

/**
 * @class Automaton
 *
 * @brief Core data structure representing finite automata of all forms:
 *        ε-NFA, NFA, DFA, co-deterministic automata, minimal DFA, etc.
 *
 * @details
 * Internally, an Automaton stores:
 *
 *   • A finite set of states Q
 *   • A finite input alphabet Σ
 *   • A transition relation δ : Q × Σ → P(Q)
 *   • A set of initial states I ⊆ Q
 *   • A set of final (accepting) states F ⊆ Q
 *
 * Transitions are represented as:
 *     map< pair<int,char>, set<int> >
 * where:
 *     (state, symbol) → { target states }
 *
 * The class provides implementations for many classical automata-theory
 * transformations:
 *   - File I/O (read/write)
 *   - Determinisation (subset construction)
 *   - Minimization (partition refinement)
 *   - Reversal of transitions (transpose automaton)
 *   - Structural isomorphism checking
 *
 * This class serves as the backbone of the entire project—
 * all higher-level algorithms (regex conversion, ENFA, NFA, DFA, minimal DFA,
 * Brzozowski’s algorithm, standardization, etc.) rely on this API.
 */
class Automaton {
public:

    /* =====================================================================
       Static Constructors and High-Level Algorithms
    ===================================================================== */

    /**
     * @brief Reads an automaton from a text file in standardized format.
     *
     * Expected sections:
     *   STATES:
     *   ALPHABET:
     *   TRANSITIONS:
     *   INITIAL_STATES:
     *   FINAL_STATES:
     *
     * @param filename Path to the automaton text file.
     * @return Constructed Automaton.
     */
    static Automaton readAutomaton(const std::string& filename);

    /**
     * @brief Performs classic **subset construction** to convert an NFA/ε-NFA
     *        into an equivalent DFA.
     *
     * Produces a deterministic automaton but not necessarily minimal.
     *
     * @param A Input NFA.
     * @return Deterministic automaton (DFA).
     */
    static Automaton determinise(const Automaton& A);

    /**
     * @brief Computes the **reverse (transpose) automaton** Aᵗ.
     *
     * Reverses all transitions and swaps initial ↔ final states.
     * Used in co-determinism analysis and Brzozowski’s algorithm.
     *
     * @param A Input automaton.
     * @return Reversed automaton Aᵗ.
     */
    static Automaton reverseTransitions(const Automaton& A);

    /**
     * @brief Minimizes a DFA using classical partition-refinement
     *        (e.g. Moore or Hopcroft algorithm depending on implementation).
     *
     * Assumes the input is already deterministic.
     *
     * @param A Deterministic automaton.
     * @return Minimal DFA equivalent to A.
     */
    static Automaton minimize(const Automaton& A);

    /**
     * @brief Checks structural **isomorphism** between two minimal DFAs.
     *
     * Automata A and B are isomorphic if a bijection f : Q_A → Q_B exists
     * such that:
     *   - f maps initial → initial
     *   - f maps final → final
     *   - f( δ_A(q,a) ) = δ_B( f(q), a )
     *
     * Optionally returns the mapping f.
     *
     * @param A First minimal DFA.
     * @param B Second minimal DFA.
     * @param mappingOut Optional pointer to store the resulting bijection.
     * @return true if A and B are isomorphic; false otherwise.
     */
    static bool isIsomorphic(const Automaton& A, const Automaton& B,
                             std::map<int, int>* mappingOut);


    /* =====================================================================
       File Output
    ===================================================================== */

    /**
     * @brief Writes the automaton to a text file using the standard internal
     *        format used throughout the project.
     *
     * @param filename Output path.
     */
    void writeAutomaton(const std::string& filename) const;


    /* =====================================================================
       Getters (const references to internal structures)
    ===================================================================== */

    const std::set<int>& getStates() const { 
        return states; 
    }

    const std::set<int>& getInitialStates() const { 
        return initialStates; 
    }

    const std::set<int>& getFinalStates() const { 
        return finalStates; 
    }

    const std::set<char>& getAlphabet() const { 
        return alphabet; 
    }

    const std::map<std::pair<int, char>, std::set<int>>& getTransitions() const { 
        return transitions; 
    }


    /* =====================================================================
       Mutators (Incremental Construction)
    ===================================================================== */

    /// Add a state to the automaton.
    void addState(int s) {
        states.insert(s);
    }

    /// Add an initial state.
    void addInitialState(int s) {
        initialStates.insert(s);
    }

    /// Add a final (accepting) state.
    void addFinalState(int s) {
        finalStates.insert(s);
    }

    /**
     * @brief Adds a transition (from --symbol--> to).
     *
     * Inserts symbol into the alphabet automatically unless symbol == '#'
     * (epsilon transition).
     */
    void addTransition(int from, char symbol, int to) {
        transitions[{from, symbol}].insert(to);
        if (symbol != '#')
            alphabet.insert(symbol);
    }

    /// Set the entire alphabet explicitly.
    void setAlphabet(const std::set<char>& a) {
        alphabet = a;
    }

    /// Removes all transitions.
    void clearTransitions() {
        transitions.clear();
    }


private:
    /* =====================================================================
       INTERNAL REPRESENTATION
    ===================================================================== */

    std::set<int> states;   ///< All states Q
    std::set<int> initialStates; ///< Initial states I
    std::set<int> finalStates;   ///< Final states F
    std::set<char> alphabet;     ///< Input alphabet Σ

    /**
     * Transition relation:
     *   (state, symbol) → set of next states
     *
     * Supports both deterministic and nondeterministic automata.
     */
    std::map<std::pair<int, char>, std::set<int>> transitions;
};

#endif
