#pragma once

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const string HEADER = string(80, '=');

struct Order{
    int orderID;
    int itemID;
    int robotID;
    int shelfNumber;
    char packingStation;
    char zone;
    string status;
    Order* next;

    Order(int thisOrderID, int thisItemID, int thisRobotID, int thisShelfNumber, char thisPackingStation, char thisZone){
        orderID = thisOrderID;
        itemID = thisItemID;
        robotID = thisRobotID;
        shelfNumber = thisShelfNumber;
        packingStation = thisPackingStation;
        zone = thisZone;
        status = "Pending";
        next = nullptr;
    }
};

class WaitingQueue{
    Order* front;
    Order* rear;
    int size;

public:
    WaitingQueue(){
        front = nullptr;
        rear = nullptr;
        size = 0;
    }

    ~WaitingQueue(){
        Order* current = front;
        while(current != nullptr){
            Order* temp = current->next;
            delete current;
            current = temp;
        }
    }

    bool isEmpty(){
        if(size == 0){
            return true;
        }
        return false;
    }

    void enqueue(Order* order){
        order->next = nullptr;

        if(rear == nullptr){
            front = order;
            rear = order;
            size++;

            cout << HEADER << endl;
            cout << "Order #" << order->orderID << " is now waiting to be processed." << endl;
            cout << HEADER << endl;

            return;
        }
        rear->next = order;
        rear = order;
        size++;

        cout << endl << HEADER << endl;
        cout << "Order #" << order->orderID << " is now waiting to be processed." << endl;
        cout << HEADER << endl;

        return;
    }

    Order* dequeue(){
        if(isEmpty()){
            cout << HEADER << endl;
            cout << "Queue empty!" << endl;
            cout << HEADER << endl;
            return nullptr;
        }
        Order* temp = front;
        front = front->next;
        size--;

        if(front == nullptr)   
        rear = nullptr; 
        return temp;
    }

    void display(){
        if(isEmpty()){
            cout << HEADER << endl;
            cout << "Queue empty!" << endl;
            cout << HEADER << endl;
            return;
        }
        Order* current = front;

        cout << HEADER << endl;
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
        cout << HEADER << endl;
        while(current != nullptr){
            cout << left
                 << setw(10) << current->orderID
                 << "| "
                 << setw(10) << current->itemID
                 << "| "
                 << setw(10) << current->shelfNumber
                 << "| "
                 << setw(12) << current->packingStation
                 << "| "
                 << setw(8)  << current->zone
                 << "| "
                 << setw(10) << (current->robotID == -1 ? "None" : to_string(current->robotID))
                 << "| "
                 << setw(15) << current->status
                 << endl;
            current = current->next;
        }
    }
};

class ProcessingQueue{
    Order** queue;
    int front;
    int rear;
    int capacity;
    int size;
    

public:
    ProcessingQueue(int maxSize){
        capacity = maxSize;
        queue = new Order*[capacity];
        front = 0;
        rear = -1;
        size = 0;
    }

    ~ProcessingQueue(){
        delete[] queue;
    }

    void enqueue(Order* order){
        if(size >= capacity){
            cout << "Queue full!" << endl;
            return;
        }
        rear = (rear + 1) % capacity;
        queue[rear] = order;
        size++;
        order->status = "Processing";

        cout << HEADER << endl;
        cout << "Order #" << order->orderID << " is now processing." << endl;
        cout << HEADER << endl;

        return;
    }

    Order* dequeue(){
        if(isEmpty()){
            cout << HEADER << endl;
            cout << "Queue empty!" << endl;
            cout << HEADER << endl;
            return nullptr;
        }
        
        Order* o = queue[front];
        front = (front + 1) % capacity;
        size--;

        return o;
    }

    void display(){
        if(isEmpty()){
            cout << HEADER << endl;
            cout << "Queue empty!" << endl;
            cout << HEADER << endl;
            return;
        }

        cout << HEADER << endl;
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
        cout << HEADER << endl;
        
        for(int i = 0; i < size; i++){
            int index = (front + i) % capacity;
            cout << left
                 << setw(10) << queue[index]->orderID
                 << "| "
                 << setw(10) << queue[index]->itemID
                 << "| "
                 << setw(10) << queue[index]->shelfNumber
                 << "| "
                 << setw(12) << queue[index]->packingStation
                 << "| "
                 << setw(8)  << queue[index]->zone
                 << "| "
                 << setw(10) << queue[index]->robotID
                 << "| "
                 << setw(15) << queue[index]->status
                 << endl;
        }
    }

    bool isEmpty(){
        return size == 0;
    }

    bool isFull(){
        return size == capacity;
    }
};

class CompletedQueue{
    Order* front;
    Order* rear;
    int size;

public:
    CompletedQueue(){
        front = nullptr;
        rear = nullptr;
        size = 0;
    }

    ~CompletedQueue(){
        Order* current = front;
        while(current != nullptr){
            Order* temp = current->next;
            delete current;
            current = temp;
        }
    }

    bool isEmpty(){
        if(size == 0){
            return true;
        }
        return false;
    }

    void enqueue(Order* order){
        order->next = nullptr;
        
        if(rear == nullptr){
            front = order;
            rear = order;
            size++;

            order->status = "Completed";
            cout << HEADER << endl;
            cout << "Order #" << order->orderID << " has been processed." << endl;
            cout << HEADER << endl;

            return;
        }
        rear->next = order;
        rear = order;
        size++;

        order->status = "Completed";
        cout << HEADER << endl;
        cout << "Order #" << order->orderID << " has been processed." << endl;
        cout << HEADER << endl;
        
        return;
    }

    void display(){
        if(isEmpty()){
            cout << HEADER << endl;
            cout << "Queue empty!" << endl;
            cout << HEADER << endl;
            return;
        }
        Order* current = front;

        cout << HEADER << endl;
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
        cout << HEADER << endl;
        while(current != nullptr){
            cout << left
                 << setw(10) << current->orderID
                 << "| "
                 << setw(10) << current->itemID
                 << "| "
                 << setw(10) << current->shelfNumber
                 << "| "
                 << setw(12) << current->packingStation
                 << "| "
                 << setw(8)  << current->zone
                 << "| "
                 << setw(10) << (current->robotID == -1 ? "None" : to_string(current->robotID))
                 << "| "
                 << setw(15) << current->status
                 << endl;
            current = current->next;
        }
    }
};

Order* createOrder();
void runTask1();