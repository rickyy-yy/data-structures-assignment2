#include "Task2.hpp"
#include "Task3.hpp"
#include <iostream>
#include <limits>
 
using namespace std;
 
CircularQueue robotQueue(10);

void runTask2() {
    
    if (robotQueue.count == 0) {
        robotQueue.loadFromCSV(ROBOTS_FILE);
    }
 
    int choice = 0;
    
    do {
        //Display menu
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Task 2: Robot Assignment Module" << endl;
        cout << string(40, '=') << endl;
        
        cout << "Do you wish to:" << endl;
        cout << "1. Assign order to a robot" << endl;
        cout << "2. Mark a robot's task as completed" << endl;
        cout << "3. Set a robot to maintenance" << endl;
        cout << "4. Restore a robot from maintenance" << endl;
        cout << "5. Display all robot statuses" << endl;
        cout << "6. Display available robots" << endl;
        cout << "7. Exit" << endl;
        
        cout << "Enter your choice (1-7): ";
        cin >> choice;
 
        //Input validation for menu choice
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000000, '\n');
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "[ERROR] Invalid input. Please enter a number between 1 and 7." << endl;
            cout << string(40, '=') << endl;
            
            continue;
        }
 
        //Menu selection
        switch (choice) {
  
        //Assign processing order to robot
        case 1: {
            //Chk whether processing queue is empty
            if (processingQueue.isEmpty()) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "[Reminder] No orders in processing queue. Use Task 1 option 2 to process an order first." << endl;
                cout << string(40, '=') << endl;
                
                break;
            }
            
            //Chk whether processing queue is full
            bool found = false;
            for (int i = 0; i < processingQueue.size; i++) {
                int index = (processingQueue.front + i) % processingQueue.capacity;
                
                if (processingQueue.queue[index]->robotID == -1) {
                    robotQueue.assignNext(processingQueue.queue[index]);
                    found = true;
                    
                    break;
                }
            }

            if (!found) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "[ERROR] No unassigned orders found in processing queue." << endl;
                cout << string(40, '=') << endl;
            }
                
            break;
        }
 
        //Mark robot task as completed
        case 2: {
            int robotID;
            cout << "Enter Robot ID: ";
            
            //Input validation
            while (!(cin >> robotID)) {
                cin.clear(); 
                cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
 
            bool found = false;
            
            Order* temp[10] = {};
            int kept = 0;
 
            // pull out all orders, find the one matching this robot
            while (!processingQueue.isEmpty()) {
                Order* o = processingQueue.dequeue();
                if (o && o->robotID == robotID && !found) {
                    completedQueue.enqueue(o); // archive it
                    found = true;
                } 
                else if (o) {
                    temp[kept++] = o; // not the right order, hold it
                }
            }
            
            //Update robot status
            for (int i = 0; i < kept; i++)
                processingQueue.enqueue(temp[i]);
 
            if (found){
                robotQueue.completeTask(robotID);

                // Integration with Task 3: robot navigates back to start
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "[Task 3] Robot " << robotID << " completed its task." << endl;
                cout << "[Task 3] Launching navigation module for return journey..." << endl;
                cout << string(40, '=') << endl;
                navigateRobot(); // Hand off to Task 3 navigation
            }
            
            else {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "[Reminder] No active order was found for Robot " << robotID << "." << endl;
                cout << string(40, '=') << endl;
            }
            
            break;
        }
 
        //Send robot to maintenance
        case 3: {
            int id; 
            cout << "Enter Robot ID to send to maintenance: "; 
            
            while (!(cin >> id)) {
                cin.clear();
                cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
            robotQueue.setMaintenance(id);
            
            break;
        }
        
        //Restore robot from maintenance
        case 4: {
            int id; 
            cout << "Enter Robot ID to restore: "; 
            
            while (!(cin >> id)) {
                cin.clear();
                cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }
            robotQueue.restoreRobot(id);
            
            break;
        }
 
        //Display all robot statuses
        case 5:  
            robotQueue.displayAll();          
            break;
        
        //Display available robots
        case 6:  
            robotQueue.displayAvailable();    
            break;
        
        //Exit program
        case 7:
            cout << "Goodbye! Exiting Task 2." << endl << endl;
            break;
        
        //Invalid menu choice
        default:
            cout << "[ERROR] Invalid choice. Please enter a number between 1 and 11." << endl;
        }
    } 
    
    //Repeat until user exits
    while (choice != 7);
}
 