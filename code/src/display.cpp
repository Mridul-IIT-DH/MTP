#include <iostream>

void display() {
    std::cout << "\nChoose one of the following options: \n"
         << "1. Proposition 3.13 \n"
         << "2. Brozozowski's Algorithm \n"
         << "3. Minimal DFA (Determinise then minimize) \n"
         << "4. Run all options 1, 2 and 3 \n"
         << "5. Check if two minimized DFAs are isomorphic \n"
         << "6. Regex to minimal DFA \n"
         << "7. Generate image of an automaton \n"
         << "0. Exit \n"
         << std::endl;
}