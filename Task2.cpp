#include "Task2.hpp"
#include <iostream>
#include <limits>
 
using namespace std;

//Function declarations
void loadItems();
void loadWaitingQueue();
void loadProcessingQueue();
void loadCompletedQueue();
int getFileLength(string filepath);

//Create robot queue with maximum size of 50
CircularQueue robotQueue(50);

void runTask2() {
    //Load robot data from CSV file if queue is empty
    if (robotQueue.count == 0) {
        robotQueue.loadFromCSV(ROBOTS_FILE);
    }

    // Initialize processingQueue capacity if entering Task2 directly
    if (processingQueue.capacity == 0) {
        int robot_count = getFileLength(ROBOTS_FILE);
        if (robot_count < 1) robot_count = 50;
        processingQueue.resize(robot_count);
    }

    // Load item list and queues if they are still empty
    if (waitingQueue.size == 0 && completedQueue.size == 0) {
        
        //Get total number of items from file
        int item_count = getFileLength(ITEMS_FILE);
        
        //Initialize item array size
        itemArray.init(item_count);

        //Load all required data
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
        
        //Get user input
        cout << "Enter your choice (1-8): ";
        cin >> choice;
 
        //Input validation for menu choice
        if (cin.fail()) {
            
            //Clear invalid input
            cin.clear();
            cin.ignore(1000000, '\n');
            
            //Display error message
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "[ERROR] Invalid input. Please enter a number between 1 and 8." << endl;
            cout << string(40, '=') << endl;
            
            continue;
        }
 
        switch (choice) {
        case 1:     //Add new robot into the system
            robotQueue.addRobot();
            break;

        case 2: {      //Delete robot using robot ID
            int id;
            cout << "Enter Robot ID to delete: ";
            
            //Validate robot ID input
            while (!(cin >> id)) {
                cin.clear(); cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
            
            robotQueue.deleteRobot(id);
            break;
        }

        case 3: {   //Turn maintenance mode on or off
            int id;
            cout << "Enter Robot ID to toggle maintenance: ";
            
            //Validate robot ID input
            while (!(cin >> id)) {
                cin.clear(); 
                cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
            
            robotQueue.toggleMaintenance(id);
            break;
        }

        case 4:     //Assign next waiting order to an avaiable robot
            robotQueue.assignNextFromWaiting();
            break;

        case 5: {   //Mark robot task a completed
            int robotID;
            cout << "Enter Robot ID: ";
            
            //Validate robot ID input
            while (!(cin >> robotID)) {
                cin.clear(); cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
            
            robotQueue.completeTask(robotID);
            break;
        }

        case 6:     //Display status of all robots
            robotQueue.displayAll();
            break;

        case 7:     //Display only available robots
            robotQueue.displayAvailable();
            break;

        case 8:     //Exit Task 2 module
            cout << "Goodbye! Exiting Task 2." << endl;
            break;

        default:    //Handle invalid menu choice
            cout << "\n[ERROR] Invalid choice. Please enter a number between 1 and 8." << endl;
        }
    } 
    
    //Repeat menu until user chooses to exit
    while (choice != 8);
}
 