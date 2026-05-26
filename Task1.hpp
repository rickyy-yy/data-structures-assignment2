#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <fstream>

using namespace std;

const string ITEMS_FILE = "items.csv";
const string ROBOTS_FILE = "robots.csv";
const string STATIONS_FILE = "stations.csv";
const string ZONES_FILE = "zones.csv";
const string HEADER = string(80, '=');

struct Item{
    int itemID;
    string itemName;
    int shelfNumber;
    char zone;
};

struct ItemArray{
    Item* array;
    int size;
    int capacity;
    int pointer;

    ItemArray() : capacity(0), size(0), array(nullptr) {}

    ItemArray(int maxSize){
        pointer = -1;
        size = 0;
        capacity = maxSize;
        array = new Item[maxSize];
    }

    ~ItemArray(){
        delete[] array;
    }

    void init(int maxSize){
        delete[] array;
        capacity = maxSize;
        size = 0;
        pointer = -1;
        array = new Item[maxSize];
    }

    bool isEmpty(){
        return size == 0;
    }

    void append(Item* item){
        if(size >= capacity){
            cout << HEADER << endl;
            cout << "Array is full!" << endl;
            cout << HEADER << endl;
            return;
        }
        pointer++;
        array[pointer] = *item;
        size++;
        recursion(array, 0, size - 1);
    }

    void display(){
        if(isEmpty()){
            cout << HEADER << endl;
            cout << "Array is empty!" << endl;
            cout << HEADER << endl;
            return;
        }
        int tempPointer = 0;
        
        cout << left
             << setw(10) << "Item ID"
             << "| " 
             << setw(10) << "Item Name"
             << "| " 
             << setw(10) << "Shelf Number"
             << "| " 
             << setw(10) << "Zone"
             << endl;;

        while(tempPointer <= pointer){
            cout << left
                 << setw(10) << array[tempPointer].itemID                 
                 << "| "
                 << setw(10) << array[tempPointer].itemName
                 << "| "
                 << setw(10) << array[tempPointer].shelfNumber
                 << "| "
                 << setw(10) << array[tempPointer].zone
                 << endl;
            tempPointer++;
        }
    }

    void recursion(Item array[], int lowerBound, int upperBound){
        if(lowerBound < upperBound){
            int pivot = partition(array, lowerBound, upperBound);
            recursion(array, lowerBound, pivot - 1);
            recursion(array, pivot + 1, upperBound);
        }
        else{
            return;
        }
    }

    int partition(Item array[], int lowerBound, int upperBound){
        int pivot = array[upperBound].itemID;
        int i = lowerBound;

        for(int j = lowerBound; j < upperBound; j++){
            if(array[j].itemID <= pivot){
                swap(array[i], array[j]);
                i++;
            }
        }
        swap(array[i], array[upperBound]);
        return i;
    }

    Item search(int queryID){
        int lowerBound = 0;
        int upperBound = size - 1;

        while(lowerBound <= upperBound){
            int mid = (lowerBound + upperBound) / 2;

            if(array[mid].itemID == queryID){
                return array[mid];
            }
            else if(array[mid].itemID < queryID){
                lowerBound = mid + 1;
            }
            else{
                upperBound = mid - 1;
            }
        }
        return Item();
    }

    bool itemExist(int queryID){
        int lowerBound = 0;
        int upperBound = size - 1;

        while(lowerBound <= upperBound){
            int mid = (lowerBound + upperBound) / 2;

            if(array[mid].itemID == queryID){
                return true;
            }
            else if(array[mid].itemID < queryID){
                lowerBound = mid + 1;
            }
            else{
                upperBound = mid - 1;
            }
        }
        return false;
    }
};

struct Robot{

};

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
                 << setw(10) << (queue[index]->robotID == -1 ? "None" : to_string(queue[index]->robotID))
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

int getFileLength(string filepath);
void loadItems(ItemArray itemArray);
Order* createOrder(ItemArray itemArray);
void runTask1();