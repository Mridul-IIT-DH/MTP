#include "../include/Automaton.h"
#include "../include/Dot.h"

#include <iostream>

using namespace std;

void display();

Automaton createNonDeterministicAutomaton(const string& inputBaseName);

void proposition313(const Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void brozozowskisAlgorithm(const Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void minimalDFA(Automaton& nonDeterministicAutomaton, const string& inputBaseName);

void checkIsomorphism();

int main() {
    string inputBaseName;
    string outputBaseName;

    while (true) {
        display();

        cout << "Enter your choice: ";

        int choice = 0;

        cin >> choice;

        if (choice == 6) {
            cout << "\n/////////////////////////\n";
            cout << "\nThank you for being here.\n";
            cout << "\n/////////////////////////\n";
            cout << endl;

            return 0;
        }

        if (choice != 5) {
            cout << "\nEnter the input automaton file name (without .txt): ";
            
            cin >> inputBaseName;
        }

        Automaton nonDeterministicAutomaton = createNonDeterministicAutomaton(inputBaseName);

        switch(choice) {
            case 1:
                proposition313(nonDeterministicAutomaton, inputBaseName);

                break;

            case 2:
                brozozowskisAlgorithm(nonDeterministicAutomaton, inputBaseName);

                break;

            case 3:
                minimalDFA(nonDeterministicAutomaton, inputBaseName);

                break;

            case 4:
                proposition313(nonDeterministicAutomaton, inputBaseName);

                brozozowskisAlgorithm(nonDeterministicAutomaton, inputBaseName);

                minimalDFA(nonDeterministicAutomaton, inputBaseName);

                break;

            case 5:
                checkIsomorphism();

                break;

            case 6:
                return 0;

            default:
                cout << "Invalid input!" << endl;
        }
    }

    return 0;
}