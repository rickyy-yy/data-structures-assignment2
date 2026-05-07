#include <iostream>
#include "Task1.hpp"
#include "Task2.hpp"
#include "Task3.hpp"
#include "Task4.hpp"
#include "Task5.hpp"

int main() {
    int choice;

    do {
        std::cout << "\n=== Main Menu ===\n";
        std::cout << "1. Task 1\n";
        std::cout << "2. Task 2\n";
        std::cout << "3. Task 3\n";
        std::cout << "4. Task 4\n";
        std::cout << "5. Task 5\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: runTask1(); break;
            case 2: runTask2(); break;
            case 3: runTask3(); break;
            case 4: runTask4(); break;
            case 5: runTask5(); break;
            case 0: std::cout << "Exiting.\n"; break;
            default: std::cout << "Invalid choice.\n"; break;
        }
    } while (choice != 0);

    return 0;
}
