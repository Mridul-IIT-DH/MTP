#include <iostream>

/*
 * display()
 *
 * Prints the main menu of the program. This function is called from main.cpp
 * in every iteration of the main loop so that the user can choose one of the
 * available operations (regex processing, automaton conversions, visualization,
 * minimization routines, etc.).
 *
 * Note:
 *  - Keep this function synchronized with the numeric options handled in main().
 *  - Only prints text; all input handling is done in main.cpp.
 */
void display() {
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
              << "10. Minimize Regex (AST + DFA canonicalization)\n"
              << "0. Exit\n"
              << std::endl;
}
