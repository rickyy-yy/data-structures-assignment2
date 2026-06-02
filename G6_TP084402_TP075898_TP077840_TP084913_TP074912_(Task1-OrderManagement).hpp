#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <fstream>

using namespace std;

const string ITEMS_FILE = "G6_TP084402_TP075898_TP077840_TP084913_TP074912_items.csv";
const string ROBOTS_FILE = "G6_TP084402_TP075898_TP077840_TP084913_TP074912_robots.csv";
const string WAITING_QUEUE_FILE = "G6_TP084402_TP075898_TP077840_TP084913_TP074912_waiting_queue.csv";
const string PROCESSING_QUEUE_FILE = "G6_TP084402_TP075898_TP077840_TP084913_TP074912_processing_queue.csv";
const string COMPLETED_QUEUE_FILE = "G6_TP084402_TP075898_TP077840_TP084913_TP074912_completed_queue.csv";
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

    ItemArray();
    ItemArray(int maxSize);
    ~ItemArray();

    void init(int maxSize);
    bool isEmpty();
    void append(Item item);
    void display();
    void recursion(Item array[], int lowerBound, int upperBound);
    int partition(Item array[], int lowerBound, int upperBound);
    Item search(int queryID);
    bool itemExist(int queryID);
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

    Order();
    Order(int thisOrderID, int thisItemID, int thisRobotID, int thisShelfNumber, char thisPackingStation, char thisZone);
};

struct WaitingQueue{
    bool loading;
    Order* front;
    Order* rear;
    int size;

    WaitingQueue();
    ~WaitingQueue();
    bool isEmpty();
    void enqueue(Order* order);
    Order* dequeue();
    void display();
    void updateFile();
    void clear();
};

struct ProcessingQueue{
    Order** queue;
    int front;
    int rear;
    int capacity;
    int size;
    bool loading;
    
    ProcessingQueue();
    ~ProcessingQueue();
    void init(int maxSize);
    void resize(int newCapacity);
    void enqueue(Order* order, int robotID);
    Order* dequeue();
    void display();
    void updateFile();
    bool isEmpty();
    bool isFull();
    void clear();
};

struct CompletedQueue{
    bool loading;
    Order* front;
    Order* rear;
    int size;

    CompletedQueue();
    ~CompletedQueue();
    bool isEmpty();
    void enqueue(Order* order);
    void display();
    void updateFile();
    void clear();
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