#include <iostream>

/**
 * @brief Displays the main program menu in the console.
 *
 * This function prints a list of numbered options that the user
 * can choose from. Each option corresponds to a specific automata-
 * related operation or algorithm implemented elsewhere in the project.
 *
 * @details
 * The menu serves as the **main entry point for user interaction**
 * in the program. It provides access to key features such as:
 *  - Running theoretical propositions
 *  - Executing DFA minimization algorithms
 *  - Checking automaton isomorphism
 *  - Converting between automata and regular expressions
 *  - Generating visual representations
 *
 * The function only **displays** the options; it does not process
 * user input or execute any actions — that logic is handled in `main.cpp`.
 *
 * @example
 * ```cpp
 * display();
 * int choice;
 * std::cin >> choice;
 * // process choice ...
 * ```
 */
void display() {
    // Print the main user options menu to the console.
    std::cout << "\nChoose one of the following options:\n"
              << "1. Proposition 3.13\n"
              << "2. Brzozowski's Algorithm\n"
              << "3. Minimal DFA (Determinise then minimize)\n"
              << "4. Run all options 1, 2, and 3\n"
              << "5. Check if two minimized DFAs are isomorphic\n"
              << "6. Regex to minimal DFA\n"
              << "7. Generate image of an automaton\n"
              << "8. NFA to Regular Expression\n"
              << "9. Standardize Regular Expression\n"
              << "0. Exit\n"
              << std::endl;
}
