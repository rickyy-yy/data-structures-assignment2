#include "Task2.hpp"
#include <iostream>
#include <limits>

using namespace std;

void runTask2() {
    RobotAssignmentSystem system(5);    //Create robot assignment system with 5 robots
    WaitingQueue pendingQueue;    //Queue for pending orders
    ProcessingQueue processingQueue(5); //Queue for processing orders
    CompletedQueue completedQueue;  //Queue for completerd orders
    
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
            cin.ignore(1000000, '\n');
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
            int itemID, shelfNumber;
            char packingStation, zone;

            cout << endl;
            cout << string(40, '=') << endl;
            cout << "Recording New Order" << endl;
            cout << string(40, '=') << endl;
            cout << "Order ID: " << nextOrderID << endl;

            //Input item id
            cout << "Enter Item ID: ";
            while (!(cin >> itemID)) {
                cin.clear(); 
                cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Item ID: ";
            }
            
            //Input shelf number
            cout << "Enter Shelf Number: ";
            while (!(cin >> shelfNumber)) {
                cin.clear(); 
                cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Shelf Number: ";
            }
            
            //Input packing station
            cout << "Enter Packing Station: "; 
            cin >> packingStation;
            
            //Input zone
            cout << "Enter Zone: ";            
            cin >> zone;

            // Construct with ALL correct values at once
            Order* o = new Order(nextOrderID++, itemID, -1, shelfNumber, packingStation, zone);

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
                cin.clear(); 
                cin.ignore(1000000, '\n');
                cout << "[ERROR] Invalid input. Please enter a valid Robot ID: ";
            }

            bool found = false;
            
            Order* temp[5] = {};
            int kept = 0;

            // pull out all orders, find the one matching this robot
            while (!processingQueue.isEmpty()) {
                Order* o = processingQueue.dequeue();
                if (o && o->robotID == robotID && !found) {
                    o->status = "Completed";   // mark the order as done
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
        case 8:  pendingQueue.display();    
            break;
        
        //Display processing orders
        case 9:  processingQueue.display(); 
            break;

        //Display completed orders
        case 10: completedQueue.display();  
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

