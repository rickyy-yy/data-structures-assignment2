#include "Task2.hpp"
#include <iostream>
#include <limits>

using namespace std;
using namespace Task1;

void runTask2() {
    RobotAssignmentSystem system(5);    //Create robot assignment system with 5 robots
    UnlimitedQueue pendingQueue;    //Queue for pending orders
    ProcessingQueue processingQueue(5); //Queue for processing orders
    UnlimitedQueue completedQueue;  //Queue for completerd orders
    
    int nextOrderID = 1;    //Auto-increment order id

    int choice = 0;
    
    do {
        //Display menu
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Task 2: Robot Assignment Module" << endl;
        cout << string(40, '=') << endl;
        
        cout << "Do you wish to:" << endl;
        cout << "1. Record a new order" << endl;
        cout << "2. Assign order to a robot" << endl;
        cout << "3. Mark a robot's task as completed" << endl;
        cout << "4. Set a robot to maintenance" << endl;
        cout << "5. Restore a robot from maintenance" << endl;
        cout << "6. Display all robot statuses" << endl;
        cout << "7. Display available robots" << endl;
        cout << "8. Display all pending orders" << endl;
        cout << "9. Display all processing orders" << endl;
        cout << "10. Display all completed orders" << endl;
        cout << "11. Exit" << endl;
        
        cout << "Enter your choice (1-11): ";
        cin >> choice;

        //Input validation for menu choice
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "[ERROR] Invalid input. Please enter a number between 1 and 11." << endl;
            cout << string(40, '=') << endl;
            
            continue;
        }

        //Menu selection
        switch (choice) {

        //Record a new order
        case 1: {   
            //Create new order dynamically
            Order* o   = new Order();
            
            //Assign unique order id
            o->orderID = nextOrderID++;

            cout << endl;
            cout << string(40, '=') << endl;
            cout << "Recording New Order" << endl;
            cout << string(40, '=') << endl;
            cout << "Order ID: " << o->orderID << endl;

            //Input item id
            cout << "Enter Item ID: ";
            while (!(cin >> o->itemID)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Item ID: ";
            }
            
            //Input shelf number
            cout << "Enter Shelf Number: ";
            while (!(cin >> o->shelfNumber)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Shelf Number: ";
            }
            
            //Input packing station
            cout << "Enter Packing Station: "; 
            cin >> o->packingStation;
            
            //Input zone
            cout << "Enter Zone: ";            
            cin >> o->zone;

            //Add order into pending queue
            pendingQueue.enqueue(o);
            
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "[OK] Order " << o->orderID << " has been recorded and is now pending." << endl;
            cout << string(40, '=') << endl;
            
            break;
        }

        //Assign pending order to robot
        case 2: {
            //Chk whether penbding queue is empty
            if (pendingQueue.isEmpty()) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "[Reminder] There are no pending orders to process right now." << endl;
                cout << string(40, '=') << endl;
                
                break;
            }
            
            //Chk whether processing queue is full
            if (processingQueue.isFull()) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "[ERROR] All robots are currently busy. No more orders can be processed." << endl;
                cout << string(40, '=') << endl;
                break;
            }

            //Remove first pending order
            Order* o = pendingQueue.dequeue();
            
            //Assign robot to order
            int id = system.assignRobot(o);

            //If robot assignment successful
            if (id != -1)
                processingQueue.enqueue(o); //MOve order into processing queue
            else
                pendingQueue.enqueue(o);    //Put order back into pending queue
            
            break;
        }

        //Mark robot task as completed
        case 3: {
            int robotID;
            cout << "Enter Robot ID: ";
            
            //Input validation
            while (!(cin >> robotID)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }

            bool found = false;
            
            //Store processing queue size
            int n = processingQueue.count;
            
            //Pass over processing queue
            for (int i = 0; i < n; i++) {
                //Remove front order temporarily
                Order* o = processingQueue.dequeue();
                
                if (!o) break;
                
                //Chk matchiing robot id
                if (o->robotID == robotID && !found) {
                    o->completed();             // mark it done
                    completedQueue.enqueue(o);  // move to completed
                    found = true;
                } 
                else {
                    processingQueue.enqueue(o); // put unmatched order back
                }
            }
            
            //Update robot status
            if (found)
                system.completeTask(robotID);
            else {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "[Reminder] No active order was found for Robot " << robotID << "." << endl;
                cout << string(40, '=') << endl;
            }
            
            break;
        }

        //Send robot to maintenance
        case 4: {
            int id; 
            cout << "Enter Robot ID to send to maintenance: "; 
            cin >> id;
            
            system.setMaintenance(id);
            
            break;
        }
        
        //Restore robot from maintenance
        case 5: {
            int id; 
            cout << "Enter Robot ID to restore: "; 
            cin >> id;
            
            system.restoreRobot(id);
            
            break;
        }

        //Display all robot statuses
        case 6:  system.displayAll();          
            break;
        
        //Display available robots
        case 7:  system.displayAvailable();    
            break;

        //Display pending orders
        case 8:  pendingQueue.displayAll();    
            break;
        
        //Display processing orders
        case 9:  processingQueue.displayAll(); 
            break;

        //Display completed orders
        case 10: completedQueue.displayAll();  
            break;
        
        //Exit program
        case 11:
            cout << "Goodbye! Exiting Task 2." << endl << endl;
            break;
        
        //Invalid menu choice
        default:
            cout << "[ERROR] Invalid choice. Please enter a number between 1 and 11." << endl;
        }
    } 
    
    //Repeat until user exits
    while (choice != 11);
}

