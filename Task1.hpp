#pragma once

#include <string>
#include <iomanip>
#include <iostream>
using namespace std;

namespace Task1 {

    struct Order{
        int orderID;
        int itemID;
        int robotID;
        int shelfNumber;
        char packingStation;
        char zone;
        string status;
        Order* next;

        Order(){
            status = "Pending";
            next = nullptr;
            robotID = -1;
        };

        void inProcess(){
            status = "Processing";
        };
        void completed(){
            status = "Completed";
        }
        void assignRobot(int assignedRobotID){
            robotID = assignedRobotID;
        }
    };

    struct UnlimitedQueue {
        Order* front;
        Order* rear;
        int count;

        UnlimitedQueue(){
            front = rear = nullptr;
            count = 0;
        }

        ~UnlimitedQueue() {
            Order* curr = front;
            while (curr != nullptr) {
                Order* temp = curr->next;
                delete curr;
                curr = temp;
            }
        }

        bool isEmpty() { return count == 0; }

        void enqueue(Order* order) {
            order->next = nullptr;
            if (isEmpty()) {
                front = rear = order;
            } else {
                rear->next = order;
                rear = order;
            }
            count++;
        }

        Order* dequeue() {
            if (isEmpty()) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "! Queue is empty." << endl;
                cout << string(40, '=') << endl;

                return nullptr;
            }

            Order* temp = front;
            front = front->next;
            if (front == nullptr) rear = nullptr;
            temp->next = nullptr;
            count--;
            return temp;
        }

        void displayAll() {
            if (isEmpty()) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "! No orders to display." << endl;
                cout << string(40, '=') << endl;
                return;
            }

            cout << endl;
            cout << string(40, '=') << endl;
            cout << "Orders in This Queue:" << endl;
            cout << string(40, '=') << endl;
            cout << endl;

            Order* curr = front;

            cout << left
                 << setw(10) << "Order ID"
                 << "| " 
                 << setw(10) << "Item ID"
                 << "| "
                 << setw(10) << "Shelf"
                 << "| "
                 << setw(12) << "Station"
                 << "| "
                 << setw(8)  << "Zone"
                 << "| "
                 << setw(10) << "Robot ID"
                 << "| "
                 << setw(15) << "Status"
                 << endl;

            cout << string(79, '-') << endl;
            while (curr != nullptr) {
                cout << left
                     << setw(10) << curr->orderID
                     << "| "
                     << setw(10) << curr->itemID
                     << "| "
                     << setw(10) << curr->shelfNumber
                     << "| "
                     << setw(12) << curr->packingStation
                     << "| "
                     << setw(8)  << curr->zone
                     << "| "
                     << setw(10) << curr->robotID
                     << "| "
                     << setw(15) << curr->status
                     << endl;

                curr = curr->next;
            }
        }

    };

    struct ProcessingQueue {
        Order* rear;
        int count;
        int capacity;

        ProcessingQueue(int cap) : rear(nullptr), count(0), capacity(cap) {}

        ~ProcessingQueue() {
            if (rear == nullptr) {
                return;
            }
            Order* curr = rear->next;
            rear->next = nullptr;
            while (curr != nullptr) {
                Order* temp = curr->next;
                delete curr;
                curr = temp;
            }
        }

        bool isEmpty() { return count == 0; }
        bool isFull()  { return count == capacity; }

        void enqueue(Order* order) {
            if (isFull()) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "! Processing queue is full. No robots available." << endl;
                cout << string(40, '=') << endl;
                return;
            }
            if (isEmpty()) {
                rear = order;
                rear->next = rear;
            } else {
                order->next = rear->next;
                rear->next = order;
                rear = order;
            }
            count++;
        }

        Order* dequeue() {
            if (isEmpty()) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "! Processing queue is empty." << endl;
                cout << string(40, '=') << endl;
                return nullptr;
            }
            Order* front = rear->next;
            if (count == 1) {
                rear = nullptr;
            } else {
                rear->next = front->next;
            }
            front->next = nullptr;
            count--;
            return front;
        }

        void displayAll() {
            if (isEmpty()) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "! No orders in processing." << endl;
                cout << string(40, '=') << endl;
                return;
            }

            cout << endl;
            cout << string(40, '=') << endl;
            cout << "Orders in This Queue:" << endl;
            cout << string(40, '=') << endl;
            cout << endl;

            cout << left
                 << setw(10) << "Order ID"
                 << "| " 
                 << setw(10) << "Item ID"
                 << "| "
                 << setw(10) << "Shelf"
                 << "| "
                 << setw(12) << "Station"
                 << "| "
                 << setw(8)  << "Zone"
                 << "| "
                 << setw(10) << "Robot ID"
                 << "| "
                 << setw(15) << "Status"
                 << endl;

            Order* curr = rear->next;

            cout << string(79, '-') << endl;

            for (int i = 0; i < count; i++) {
                cout << left
                     << setw(10) << curr->orderID
                     << "| "
                     << setw(10) << curr->itemID
                     << "| "
                     << setw(10) << curr->shelfNumber
                     << "| "
                     << setw(12) << curr->packingStation
                     << "| "
                     << setw(8)  << curr->zone
                     << "| "
                     << setw(10) << curr->robotID
                     << "| "
                     << setw(15) << curr->status
                     << endl;
                curr = curr->next;
            }
        }

    };

    void runTask1();
}
