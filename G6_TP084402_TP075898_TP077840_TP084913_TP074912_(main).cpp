#include <iostream>
#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task1-OrderManagement).hpp"
#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task2-RobotManagement).hpp"
#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task3-RobotStack).hpp"
#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task4-ItemManagement).hpp"
#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task5-WarehouseMapping).hpp"
#include <limits>

using namespace std;

int main() {
    int choice;

    do {
        cout << string(40, '=') << endl;
        cout << "Data Structures Assignment 2" << endl;
        cout << string(40, '=') << endl;

        cout << "Please select a task to run:" << endl;
        cout << "1. Order Management" << endl;
        cout << "2. Robot Management" << endl;
        cout << "3. Robot Navigation" << endl;
        cout << "4. Item Search" << endl;
        cout << "5. Warehouse Layout and Management" << endl;
        cout << "6. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        if(cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "! Invalid input. Please enter a number between 1 and 6." << endl;
            cout << string(40, '=') << endl;
            cout << endl;
        }
        else {
            switch (choice) {
            case 1: 
                runTask1(); 
                break;
            case 2: 
                runTask2(); 
                break;
            case 3: 
                runTask3(); 
                break;
            case 4: 
                Task4::runTask4(); 
                break;
            case 5: 
                Task5::runTask5(); 
                break;
            case 6: 
                cout << "See you next time!" << endl; 
                break;
            default:
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "! Invalid choice. Please enter a number between 1 and 6." << endl;
                cout << string(40, '=') << endl;
                cout << endl;
            }
        }
        
    } while (choice != 6);

    return 0;
}
