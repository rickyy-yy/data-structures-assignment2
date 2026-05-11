#include "Task1.hpp"
#include <iostream>
#include <limits>

using namespace std;
using namespace Task1;

namespace Task1 {
    Order* createOrder(int& nextOrderID){
        Order* newOrder = new Order();
        newOrder->orderID = nextOrderID++;

        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Recording New Order" << endl;
        cout << string(40, '=') << endl;

        cout << "Order ID: " << newOrder->orderID << endl;

        cout << "Enter Item ID: ";
        while (!(cin >> newOrder->itemID)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter Item ID: ";
        }

        cout << "Enter Shelf Number: ";
        while (!(cin >> newOrder->shelfNumber)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter Shelf Number: ";
        }

        cout << "Enter Packing Station: ";
        cin >> newOrder->packingStation;

        cout << "Enter Zone: ";
        cin >> newOrder->zone;

        return newOrder;
    }

    void runTask1() {
        UnlimitedQueue pendingQueue;
        ProcessingQueue processingQueue(5); // Assuming we have 5 robots available
        UnlimitedQueue completedQueue;
        int nextRobotID = 1;
        int nextOrderID = 1;

        int choice = 0;
        do {
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "Task 1: Queue Implementation" << endl;
            cout << string(40, '=') << endl;

            cout << "Do you wish to:" << endl;
            cout << "1. Record a new order" << endl;
            cout << "2. Process an order" << endl;
            cout << "3. Display all pending orders" << endl;
            cout << "4. Display all processing orders" << endl;
            cout << "5. Display all completed orders" << endl;
            cout << "6. Exit" << endl;

            cout << "Enter your choice (1-6): ";
            cin >> choice;

            if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "! Invalid input. Please enter a number between 1 and 6." << endl;
            cout << string(40, '=') << endl;
            }
            else {
                switch(choice){
                case 1:
                    pendingQueue.enqueue(createOrder(nextOrderID));
                    cout << endl;
                    cout << string(40, '=') << endl;
                    cout << "! Order added to queue." << endl;
                    cout << string(40, '=') << endl;
                    break;
                case 2:
                    if (pendingQueue.isEmpty()) {
                        cout << endl;
                        cout << string(40, '=') << endl;
                        cout << "! No orders to process." << endl;
                        cout << string(40, '=') << endl;
                    } else if (processingQueue.isFull()) {
                        cout << endl;
                        cout << string(40, '=') << endl;
                        cout << "! All robots are busy. Cannot process more orders." << endl;
                        cout << string(40, '=') << endl;
                    } else {
                        Order* order = pendingQueue.dequeue();
                        order->assignRobot(nextRobotID++);
                        order->inProcess();
                        processingQueue.enqueue(order);
                        cout << endl;
                        cout << string(40, '=') << endl;
                        cout << "! Order " << order->orderID << " is now being processed by Robot " << order->robotID << "." << endl;
                        cout << string(40, '=') << endl;
                    }
                    break;
                case 3:
                    pendingQueue.displayAll();
                    break;
                case 4:
                    processingQueue.displayAll();
                    break;
                case 5:
                    completedQueue.displayAll();
                    break;
                case 6:
                    cout << "Exiting Task 1." << endl;
                    cout << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }  
        } while (choice != 6);
    }
}
