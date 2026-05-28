#pragma once
#include "Task1.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

//Stores information about a delivery robot
struct Robot{
    int robotID;    //unique identifier for robot
    string status;  //Robot status: Available, Busy and Mainenance
    int assignedOrderId;    //current assigned order id
    int totalTaskDone;  //total completed tasks

    //Default constructor
    Robot (){
        robotID = 0;    
        status = "Available";   
        assignedOrderId = -1;
        totalTaskDone = 0;
    }
};

//Circular queue used to manage robots
struct CircularQueue{
    Robot* slots;   //dynamic array for queue storage
    int cap, front, rear, count, currentTurn;

    //Constructor
    //Initialize circular queue with given capacity
    CircularQueue(int capacity) : cap(capacity), front(0), rear(-1), count(0), currentTurn(0){
        slots = new Robot [cap];
    }
    
    //Destructor
    //Release allocated memory
    ~CircularQueue(){
        delete[] slots;
    }

    //Check whether queue is empty
    bool IsEmpty(){
        return count == 0;
    }

    //Chk whether queue is full
    bool IsFull(){
        return count == cap;
    }

    //Add a robot to the rear of the queue
    void enqueue(Robot r){
        
        //Prevent insertion if queue is full
        if(IsFull()){
            cout << "\n[ERROR] Queue is full! Cannot add on more robots." << endl;
            return;
        }

        //Move rear position circularly
        rear = (rear + 1) % cap;
        
        //Insert robot into queue
        slots[rear] = r;
        
        //Increase queue size
        count ++;

        cout << "\n Robot " << r.robotID << "\n added to queue." << endl;
    }

    //Remove robot from the front of the queue
    Robot dequeue(){
        Robot r;
        
        //Prevent removal if queue is empty
        if (IsEmpty()){
            cout << "\n[ERROR] Queue is empty. Nothing to remove." << endl;
            return r;
        }

        //Retrieve robot at front
        r = slots[front];
        
        //Move front pointer circularly
        front = (front + 1) % cap;
        
        //Reduce queue size
        count --;
        
        return r;
    }

    void assignNextFromWaiting(){
        if(waitingQueue.isEmpty()){
            cout << endl;
            cout << string(40,'=') << endl;
            cout << "[Reminder] No orders in the waiting queue." << endl;
            cout << string(40,'=') << endl;
            return;
        }
        if(processingQueue.isFull()){
            cout << endl;
            cout << string(40,'=') << endl;
            cout << "[Reminder] Processing queue is full. All robots are currently busy." << endl;
            cout << string(40,'=') << endl;
            return;
        }

        // Find an available robot (round-robin)
        int assignedSlot = -1;
        for(int attempt = 0; attempt < count; attempt++){
            int index = (front + currentTurn) % cap;
            currentTurn = (currentTurn + 1) % count;
            if(slots[index].status == "Available"){
                assignedSlot = index;
                break;
            }
        }

        if(assignedSlot == -1){
            cout << endl;
            cout << string(40,'=') << endl;
            cout << "[Reminder] All robots are currently busy or under maintenance." << endl;
            cout << "           Please complete a task or restore a robot first." << endl;
            cout << string(40,'=') << endl;
            return;
        }

        // Dequeue order from waitingQueue (also rewrites waiting_queue.csv)
        Order* order = waitingQueue.dequeue();
        if(order == nullptr) return;

        // Assign robot to order
        slots[assignedSlot].status = "Busy";
        slots[assignedSlot].assignedOrderId = order->orderID;
        order->robotID = slots[assignedSlot].robotID;
        order->status = "Processing";

        // Place order into processingQueue (in-memory)
        processingQueue.enqueue(order);

        cout << endl;
        cout << string(40,'=') << endl;
        cout << "[OK] Order " << order->orderID
             << " assigned to Robot " << slots[assignedSlot].robotID << "." << endl;
        cout << string(40,'=') << endl;

        saveToCSV(ROBOTS_FILE);
    }

 
    // Mark a robot's task as completed and set it back to Available
    bool completeTask(int robotID) {
        int robotSlot = -1;
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].robotID == robotID) { 
                robotSlot = index;
                break;
            }
        }

        if(robotSlot == -1){
            cout << "\n[ERROR] Robot " << robotID << " not found in queue.\n";
            return false;
        }
        if(slots[robotSlot].status != "Busy"){
            cout << "\n[Reminder] Robot " << robotID << " is not currently busy.\n";
            return false;
        }

        // Find and pull the matching order out of processingQueue
        Order* doneOrder = nullptr;
        Order* temp[50]  = {};
        int    kept      = 0;

        while(!processingQueue.isEmpty()){
            Order* o = processingQueue.dequeue();
            if(o && o->robotID == robotID && doneOrder == nullptr){
                doneOrder = o;
            } else if(o){
                temp[kept++] = o;
            }
        }
        // Re-insert the orders that belong to other robots
        for(int j = 0; j < kept; j++)
            processingQueue.enqueue(temp[j]);

        if(doneOrder == nullptr){
            cout << "\n[ERROR] Could not find order assigned to Robot " << robotID << ".\n";
            // Still free the robot so it doesn't stay locked
            slots[robotSlot].status          = "Available";
            slots[robotSlot].assignedOrderId = -1;
            slots[robotSlot].totalTaskDone++;
            saveToCSV(ROBOTS_FILE);
            return false;
        }

        // Move order to completedQueue (writes completed_queue.csv)
        completedQueue.enqueue(doneOrder);

        // Free the robot
        slots[robotSlot].status          = "Available";
        slots[robotSlot].assignedOrderId = -1;
        slots[robotSlot].totalTaskDone++;

        cout << "\n[OK] Robot " << robotID
             << " has completed its task and is now available.\n";

        saveToCSV(ROBOTS_FILE);

        // Auto-assign next waiting order if any exist
        if(!waitingQueue.isEmpty()){
            cout << "\n[Reminder] Checking waiting queue for next order..." << endl;
            assignNextFromWaiting();
        }

        return true;
    }

    // Set a robot to Maintenance mode (only if it is currently Available)
    bool toggleMaintenance(int robotID) {
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].robotID == robotID) {
                if (slots[index].status == "Busy") {
                    cout << "\n[Reminder] Robot " << robotID
                         << " is still busy. Complete its task first.\n";
                    return false;
                }
                
                if (slots[index].status == "Available") {
                    slots[index].status = "Maintenance";
                    cout << "\n[OK] Robot " << robotID << " has been sent to maintenance.\n";
                }

                else{
                    slots[index].status = "Available";
                    cout << "\n[OK] Robot " << robotID << " has been restored and is now available.\n";
                } 
                saveToCSV(ROBOTS_FILE);
                return true;
            }
        }
        cout << "\n[ERROR] Robot " << robotID << " not found in queue.\n";
        return false;
    }
 
    //Print out the robot that currently in the queue
    void displayAll(){
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Robots in Circular Queue:" << endl;
        cout << string(40, '=') << endl << endl;

        //Chk whether queue is empty
        if (IsEmpty()){
            cout << "\n[ERROR] No robots are currently in the queue now." << endl;
            return;
        }
        
        //Table header
        cout << left
             << setw(10) << "Robot ID" << "| "
             << setw(14) << "Status"   << "| "
             << setw(12) << "Order ID" << "| "
             << "Tasks Done" << endl;
        
        cout << string(50, '-') << endl;
        
        //Walk through the queue from front to rear
        for (int i = 0; i < count; i++) {
            //Calculate actual index circularly
            int index = (front + i) % cap;
            
            //Display 'None' if no order assigned
            string ord = (slots[index].assignedOrderId == -1) ? "None" : to_string(slots[index].assignedOrderId);
            
            //Print robot details
            cout << left
                 << setw(10) << slots[index].robotID << "| "
                 << setw(14) << slots[index].status  << "| "
                 << setw(12) << ord                << "| "
                 << slots[index].totalTaskDone << endl;
        }
    }

    // Display only robots with "Available" status
    void displayAvailable() {
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Available Robots:" << endl;
        cout << string(40, '=') << endl;
        bool found = false;
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].status == "Available") {
                cout << "  -> Robot " << slots[index].robotID
                     << " (tasks completed: " << slots[index].totalTaskDone << ")" << endl;
                found = true;
            }
        }
        if (!found)
            cout << "  No robots are available right now." << endl;
    }
 
    // Load robots from a CSV file (robotID,status)
    // Capitalises the status string to match internal format
    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "\n[ERROR] Could not open file: " << filename << endl;
            return;
        }
 
        string line;
        getline(file, line); // Skip header row
 
        int loaded = 0;
        while (getline(file, line) && !IsFull()) {
            // Remove Windows-style carriage return if present
            if (!line.empty() && line.back() == '\r') line.pop_back();
 
            stringstream ss(line);
            string idStr, statusStr;
            getline(ss, idStr,     ',');
            getline(ss, statusStr, ',');
 
            if (idStr.empty()) continue;
 
            // Capitalise first letter of status to match "Available" / "Busy" / "Maintenance"
            if (!statusStr.empty()) {
                statusStr[0] = toupper(statusStr[0]);
                for (int i = 1; i < (int)statusStr.size(); i++)
                    statusStr[i] = tolower(statusStr[i]);
            }

            if (statusStr != "Available" && statusStr != "Maintenance")
                statusStr = "Available";
 
            Robot r;
            r.robotID = stoi(idStr);
            r.status  = statusStr;
 
            // Advance rear and insert directly (bypasses console output of enqueue)
            rear = (rear + 1) % cap;
            slots[rear] = r;
            count++;
            loaded++;
        }
        file.close();
        cout << "\n[OK] Loaded " << loaded << " robot(s) from " << filename << "." << endl;
    }

    void saveToCSV(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "\n[ERROR] Could not open file for writing: " << filename << endl;
            return;
        }
        file << "robotID,status\n";
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            string savedStatus = (slots[index].status == "Busy") ? "Available" : slots[index].status;
            file << slots[index].robotID << "," << savedStatus << "\n";
        }
        file.close();
    }

    void addRobot() {
        if (IsFull()) {
            cout << "\n[ERROR] Cannot add robot — queue is at full capacity (" << cap << ")." << endl;
            return;
        }
        int maxID = 0;
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].robotID > maxID)
                maxID = slots[index].robotID;
        }
        Robot r;
        r.robotID = maxID + 1;
        r.status  = "Available";
        rear = (rear + 1) % cap;
        slots[rear] = r;
        count++;
        cout << "\n[OK] Robot " << r.robotID << " added and set to Available." << endl;
        saveToCSV(ROBOTS_FILE);
    }

    void deleteRobot(int robotID) {
        int found = -1;
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].robotID == robotID) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            cout << "\n[ERROR] Robot " << robotID << " not found." << endl;
            return;
        }
        int index = (front + found) % cap;
        if (slots[index].status == "Busy") {
            cout << "\n[Reminder] Robot " << robotID
                 << " is currently busy. Complete its task before deleting." << endl;
            return;
        }
        Robot* newSlots = new Robot[cap];
        int newCount = 0;
        for (int i = 0; i < count; i++) {
            int idx = (front + i) % cap;
            if (slots[idx].robotID != robotID)
                newSlots[newCount++] = slots[idx];
        }
        delete[] slots;
        slots  = newSlots;
        front  = 0;
        rear   = (newCount == 0) ? -1 : newCount - 1;
        count  = newCount;
        currentTurn = (count == 0) ? 0 : currentTurn % count;
        cout << "\n[OK] Robot " << robotID << " has been deleted." << endl;
        saveToCSV(ROBOTS_FILE);  
    }
};

extern CircularQueue robotQueue;
void runTask2();

