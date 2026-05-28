#include "Task2.hpp"
#include "Task3.hpp"
#include <iostream>
#include <limits>
 
using namespace std;
void loadItems();
void loadWaitingQueue();
void loadCompletedQueue();
int getFileLength(string filepath);

CircularQueue robotQueue(50);

void runTask2() {
    
    if (robotQueue.count == 0) {
        robotQueue.loadFromCSV(ROBOTS_FILE);
    }

    // Initialize processingQueue capacity if entering Task2 directly
    if (processingQueue.capacity == 0) {
        int robot_count = getFileLength(ROBOTS_FILE);
        if (robot_count < 1) robot_count = 50;
        processingQueue.resize(robot_count);
    }

    // Load queues and items if not already loaded
    if (waitingQueue.size == 0 && completedQueue.size == 0) {
        int item_count = getFileLength(ITEMS_FILE);
        itemArray.init(item_count);
        loadItems();
        loadWaitingQueue();
        loadCompletedQueue();
    }
 
    int choice = 0;
    
    do {
        //Display menu
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Task 2: Robot Assignment Module" << endl;
        cout << string(40, '=') << endl;
        
        cout << "Do you wish to:" << endl;
         cout << "1. Add robot\n";
        cout << "2. Delete robot\n";
        cout << "3. Toggle robot maintenance mode\n";
        cout << "4. Assign order to a robot\n";
        cout << "5. Mark a robot's task as completed\n";
        cout << "6. Display all robot statuses\n";
        cout << "7. Display available robots\n";
        cout << "8. Exit\n";
        
        cout << "Enter your choice (1-8): ";
        cin >> choice;
 
        //Input validation for menu choice
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000000, '\n');
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "[ERROR] Invalid input. Please enter a number between 1 and 8." << endl;
            cout << string(40, '=') << endl;
            
            continue;
        }
 
        switch (choice) {
        case 1:
            robotQueue.addRobot();
            break;

        case 2: {
            int id;
            cout << "Enter Robot ID to delete: ";
            while (!(cin >> id)) {
                cin.clear(); cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
            robotQueue.deleteRobot(id);
            break;
        }

        case 3: {
            int id;
            cout << "Enter Robot ID to toggle maintenance: ";
            while (!(cin >> id)) {
                cin.clear(); cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
            robotQueue.toggleMaintenance(id);
            break;
        }

        case 4:
            robotQueue.assignNextFromWaiting();
            break;

        case 5: {
            int robotID;
            cout << "Enter Robot ID: ";
            while (!(cin >> robotID)) {
                cin.clear(); cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
            if (robotQueue.completeTask(robotID)) {
                cout << "\n" << string(40, '=') << endl;
                cout << "[Task 3] Robot " << robotID << " completed its task." << endl;
                cout << "[Task 3] Launching navigation module for return journey..." << endl;
                cout << string(40, '=') << endl;
                navigateRobot();
            }
            break;
        }

        case 6:
            robotQueue.displayAll();
            break;

        case 7:
            robotQueue.displayAvailable();
            break;

        case 8:
            cout << "Goodbye! Exiting Task 2." << endl;
            break;

        default:
            cout << "\n[ERROR] Invalid choice. Please enter a number between 1 and 8." << endl;
        }
    } 
    
    //Repeat until user exits
    while (choice != 8);
}
 