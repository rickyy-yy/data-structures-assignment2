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
const string WAITING_QUEUE_FILE = "waiting_queue.csv";
const string PROCESSING_QUEUE_FILE = "processing_queue.csv";
const string COMPLETED_QUEUE_FILE = "completed_queue.csv";
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

    ItemArray(){
        capacity = 0;
        size = 0;
        pointer = -1;
        array = nullptr;
    }

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

    void append(Item item){
        if(size >= capacity){
            cout << HEADER << endl;
            cout << "Array is full!" << endl;
            cout << HEADER << endl;
            return;
        }
        pointer++;
        array[pointer] = item;
        size++;
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
             << endl;
             
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
        if(queryID < 1){
            return Item();
        }
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
        orderID = -1;
        itemID = -1;
        robotID = -1;
        shelfNumber = -1;
        packingStation = 'A';
        zone = 'A';
        status = "Pending";
        next = nullptr;
    }

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

struct WaitingQueue{
    bool loading;
    Order* front;
    Order* rear;
    int size;

    WaitingQueue(){
        loading = false;
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

            if(loading){
                return;
            }

            cout << endl << HEADER << endl;
            cout << "Order #" << order->orderID << " is now waiting to be processed." << endl;
            cout << HEADER << endl;
            
            updateFile();

            return;
        }
        rear->next = order;
        rear = order;
        size++;

        if(loading){
            return;
        }

        cout << endl << HEADER << endl;
        cout << "Order #" << order->orderID << " is now waiting to be processed." << endl;
        cout << HEADER << endl;

        updateFile();

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
        updateFile();

        if(front == nullptr){
            rear = nullptr;  
        }
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

    void updateFile(){
        ofstream waitingQueueFile(WAITING_QUEUE_FILE);

        if(!waitingQueueFile.is_open()){
            cout << HEADER << endl;
            cout << "Waiting queue file can't be open!" << endl;
            cout << HEADER << endl;
            return;
        }

        if(!isEmpty()){
            waitingQueueFile << "orderID,itemID,shelfNumber,zone,packingStation" << "\n";
            Order* current = front;
            while(current != nullptr){
                waitingQueueFile << current->orderID << "," << current->itemID << "," << current->shelfNumber << "," << current->zone << "," << current->packingStation << "\n";
                current = current->next;
            }
        }
        else{
            waitingQueueFile << "orderID,itemID,shelfNumber,zone,packingStation" << "\n";
        }

        waitingQueueFile.close();
    }

    void clear(){
        Order* current = front;
        while(current != nullptr){
            Order* temp = current->next;
            delete current;
            current = temp;
        }
        front = nullptr;
        rear = nullptr;
        size = 0;
    }
};

struct ProcessingQueue{
    Order** queue;
    int front;
    int rear;
    int capacity;
    int size;
    bool loading;
    
    ProcessingQueue(){
        capacity = 0;
        queue = nullptr;
        front = 0;
        rear = -1;
        size = 0;
        loading = false;
    }

    ~ProcessingQueue(){
        for(int i = 0; i < size; i++){
            int index  = (front + i) % capacity;
            delete queue[index];
        }
        delete[] queue;
    }

    void init(int maxSize){
        for(int i = 0; i < size; i++){
            int index  = (front + i) % capacity;
            delete queue[index];
        }
        delete[] queue;

        capacity = maxSize;
        queue = new Order*[capacity];
        front = 0;
        rear = -1;
        size = 0;
    }

    void resize(int newCapacity){
        if(capacity == 0){
            init(newCapacity);
            return;
        }
        if(newCapacity == capacity){
            return;
        }
        if(newCapacity < size){
            newCapacity = size;
        }

        Order** newQueue = new Order*[newCapacity];

        for(int i = 0; i < size; i++){
            int index = (front + i) % capacity;
            newQueue[i] = queue[index];
        }

        delete[] queue;

        queue = newQueue;
        capacity = newCapacity;
        front = 0;
        if(size == 0){
            rear = -1;
        }
        else{
            rear = size - 1;
        }
        
    }

    void enqueue(Order* order, int robotID){
        if(size >= capacity){
            cout << "Queue full!" << endl;
            return;
        }
        rear = (rear + 1) % capacity;
        queue[rear] = order;
        size++;
        order->status = "Processing";

        if(order->robotID <= 0){
            order->robotID = robotID;
        }

        if(loading){
            return;
        }

        updateFile();

        cout << HEADER << endl;
        cout << "Order #" << order->orderID << " is now assigned to Robot #" << order->robotID << endl;
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

        updateFile();

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

    void updateFile(){
        ofstream processingQueueFile(PROCESSING_QUEUE_FILE);

        if(!processingQueueFile.is_open()){
            cout << HEADER << endl;
            cout << "Processing queue file can't be open!" << endl;
            cout << HEADER << endl;
            return;
        }

        if(!isEmpty()){
            processingQueueFile << "orderID,itemID,shelfNumber,zone,packingStation,robotID" << "\n";
            for(int i = 0; i < size; i++){
                int index = (front + i) % capacity;
                processingQueueFile << queue[index]->orderID << "," << queue[index]->itemID << "," << queue[index]->shelfNumber << "," << queue[index]->zone << "," << queue[index]->packingStation << "," << queue[index]->robotID << "\n";
            }
        }
        else{
            processingQueueFile << "orderID,itemID,shelfNumber,zone,packingStation,robotID" << "\n";
        }
        processingQueueFile.close();
    }

    bool isEmpty(){
        return size == 0;
    }

    bool isFull(){
        return size == capacity;
    }

    void clear(){
        for(int i = 0; i< size; i++){
            int index = (front + i) % capacity;
            delete queue[index];
        }

        front = 0;
        rear = -1;
        size = 0;
    }
};

struct CompletedQueue{
    bool loading;
    Order* front;
    Order* rear;
    int size;

public:
    CompletedQueue(){
        loading = false;
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

            if(loading){
                return;
            }

            cout << HEADER << endl;
            cout << "Order #" << order->orderID << " has been processed." << endl;
            cout << HEADER << endl;

            updateFile();

            return;
        }
        rear->next = order;
        rear = order;
        size++;

        order->status = "Completed";

        if(loading){
                return;
            }

        cout << HEADER << endl;
        cout << "Order #" << order->orderID << " has been processed." << endl;
        cout << HEADER << endl;
        
        updateFile();
        
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

    void updateFile(){
        ofstream completedQueueFile(COMPLETED_QUEUE_FILE);

        if(!completedQueueFile.is_open()){
            cout << HEADER << endl;
            cout << "Completed queue file can't be open!" << endl;
            cout << HEADER << endl;
            return;
        }

        if(!isEmpty()){
            completedQueueFile << "orderID,itemID,shelfNumber,zone,packingStation,robotID" << "\n";
            Order* current = front;
            while(current != nullptr){
                completedQueueFile << current->orderID << "," << current->itemID << "," << current->shelfNumber << "," << current->zone << "," << current->packingStation << "," << current->robotID << "\n";
                current = current->next;
            }
        }
        else{
            completedQueueFile << "orderID,itemID,shelfNumber,zone,packingStation,robotID" << "\n";
        }

        completedQueueFile.close();
    }

    void clear(){
        Order* current = front;
        while(current != nullptr){
            Order* temp = current->next;
            delete current;
            current = temp;
        }
        front = nullptr;
        rear = nullptr;
        size = 0;
    }
};

extern int nextOrderID;
extern ItemArray itemArray;
extern WaitingQueue waitingQueue;
extern ProcessingQueue processingQueue;
extern CompletedQueue completedQueue;

int getFileLength(string filepath);
void loadItems();
Order* createOrder();
void runTask1();