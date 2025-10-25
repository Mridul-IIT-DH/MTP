#include "../include/Automaton.h"
#include "../include/Dot.h"
#include "../include/RegexENFA.h"

#include <iostream>

using namespace std;

void display();

Automaton createNonDeterministicAutomaton(const string& inputBaseName);

void proposition313(const Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void brozozowskisAlgorithm(const Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void checkIsomorphism();

void regexToMinimalDFA();

void automatonToImage(const string& inputBaseName);

int main() {
    string inputBaseName;

    while (true) {
        display();

        cout << "Enter your choice: ";
        int choice = -1;
        cin >> choice;

        if (choice == 0) {
            cout << "\n/////////////////////////\n";
            cout << "\nThank you for being here.\n";
            cout << "\n/////////////////////////\n";
            cout << endl;

            return 0;
        } else if (choice == 1) {
            cout << "\nEnter the input automaton file name (without .txt): ";
            
            cin >> inputBaseName;

            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            
            proposition313(nonDeterministicAutomaton, inputBaseName);
        } else if (choice == 2) {
            cout << "\nEnter the input automaton file name (without .txt): ";

            cin >> inputBaseName;
            
            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            
            brozozowskisAlgorithm(nonDeterministicAutomaton, inputBaseName);
        } else if (choice == 3) {
            cout << "\nEnter the input automaton file name (without .txt): ";

            cin >> inputBaseName;
            
            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            
            minimalDFA(nonDeterministicAutomaton, inputBaseName);
        } else if (choice == 4) {
            cout << "\nEnter the input automaton file name (without .txt): ";
            
            cin >> inputBaseName;
            
            Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);
            
            proposition313(nonDeterministicAutomaton, inputBaseName);
            
            brozozowskisAlgorithm(nonDeterministicAutomaton, inputBaseName);
            
            minimalDFA(nonDeterministicAutomaton, inputBaseName);
        } else if (choice == 5) {
            checkIsomorphism();
        } else if (choice == 6) {
            regexToMinimalDFA();
        } else if (choice == 7) {
            cout << "\nEnter the input automaton file name (without .txt): ";

            cin >> inputBaseName;

            automatonToImage(inputBaseName);
        } else {
            cout << "Invalid input!" << endl;
        }
    }

    return 0;
}