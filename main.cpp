#include <iostream>
#include "Task1.hpp"
#include "Task2.hpp"
#include "Task3.hpp"
#include "Task4.hpp"
#include "Task5.hpp"

using namespace std;
using namespace Task1;

int main() {
    int choice;

    do {
        cout << string(40, '=') << endl;
        cout << "Data Structures Assignment 2" << endl;
        cout << string(40, '=') << endl;

        cout << "Please select a task to run:" << endl;
        cout << "1. Task 1" << endl;
        cout << "2. Task 2" << endl;
        cout << "3. Task 3" << endl;
        cout << "4. Task 4" << endl;
        cout << "5. Task 5" << endl;
        cout << "6. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

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
                runTask4(); 
                break;
            case 5: 
                runTask5(); 
                break;
            case 6: 
                cout << "See you next time!" << endl; 
                break;
            default: 
                cout << "Invalid choice. Please try again." << endl; 
                break;
        }
    } while (choice != 6);

    return 0;
}
