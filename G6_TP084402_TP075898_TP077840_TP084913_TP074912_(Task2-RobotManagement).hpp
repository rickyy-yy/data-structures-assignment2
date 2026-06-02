#pragma once
#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task1-OrderManagement).hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

//Stores robot information
struct Robot{
    int robotID;    //unique robot ID
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

//Circular queue used to manage all robots
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

    //Add robot into queue
    void enqueue(Robot r){
        
        //Prevent adding if queue is already full
        if(IsFull()){
            cout << "\n[ERROR] Queue is full! Cannot add on more robots." << endl;
            return;
        }

        //Move rear pointer circularly
        rear = (rear + 1) % cap;
        
        //Store robot into queue
        slots[rear] = r;
        
        //Increase robot count
        count ++;

        cout << "\n Robot " << r.robotID << " added to queue." << endl;
    }

    //Remove robot from the front of the queue
    Robot dequeue(){
        Robot r;
        r.robotID = -1;
        
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

    //Assign next waiting order to an available robot
    void assignNextFromWaiting(){
        
        //Chk if waiting queue is empty
        if(waitingQueue.isEmpty()){
            cout << endl;
            cout << string(40,'=') << endl;
            cout << "[Reminder] No orders in the waiting queue." << endl;
            cout << string(40,'=') << endl;
            return;
        }
        
        //Chk if all processing slots are occupied
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
            
            //Move turn pointer
            currentTurn = (currentTurn + 1) % count;
            
            //Chk if robot is available
            if(slots[index].status == "Available"){
                assignedSlot = index;
                break;
            }
        }

        //No available robot found
        if(assignedSlot == -1){
            cout << endl;
            cout << string(40,'=') << endl;
            cout << "[Reminder] All robots are currently busy or under maintenance." << endl;
            cout << "           Please complete a task or restore a robot first." << endl;
            cout << string(40,'=') << endl;
            return;
        }

        // Remove order from waitingQueue
        Order* order = waitingQueue.dequeue();
        if(order == nullptr) return;

        // Assign robot to order
        slots[assignedSlot].status = "Busy";
        slots[assignedSlot].assignedOrderId = order->orderID;
        order->robotID = slots[assignedSlot].robotID;
        order->status = "Processing";

        // Place order into processingQueue
        processingQueue.enqueue(order, slots[assignedSlot].robotID);

        cout << endl;
        cout << string(40,'=') << endl;
        cout << "[OK] Order " << order->orderID
             << " assigned to Robot " << slots[assignedSlot].robotID << "." << endl;
        cout << string(40,'=') << endl;

        //Save updated robot data
        saveToCSV(ROBOTS_FILE);
    }

    // Mark a robot's task as completed and set it back to Available
    bool completeTask(int robotID) {
        int robotSlot = -1;
        
        //Search  robot by ID
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].robotID == robotID) { 
                robotSlot = index;
                break;
            }
        }

        //Robot not found
        if(robotSlot == -1){
            cout << "\n[ERROR] Robot " << robotID << " not found in queue.\n";
            return false;
        }
        
        //Robot is not busy
        if(slots[robotSlot].status != "Busy"){
            cout << "\n[Reminder] Robot " << robotID << " is not currently busy.\n";
            return false;
        }

        // Find and pull the matching order out of processingQueue
        Order* doneOrder = nullptr;
        
         //temp array to store remaining orders
        int tempSize = processingQueue.capacity > 0 ? processingQueue.capacity : 50;
        Order** temp = new Order*[tempSize]();
        int kept = 0;

       //Remove all orders from processing queue
        while(!processingQueue.isEmpty()){
            Order* o = processingQueue.dequeue();
            
            //Find order handled by selected robot
            if(o && o->robotID == robotID && doneOrder == nullptr){
                doneOrder = o;
            } 
            
            //Keep other orders temporarily
            else if(o){
                temp[kept++] = o;
            }
        }

        // Re-insert the orders that belong to other robots
        for(int j = 0; j < kept; j++)
            processingQueue.enqueue(temp[j], temp[j]->robotID);

        delete[] temp;
        
        //Order not found
        if(doneOrder == nullptr){
            cout << "\n[ERROR] Could not find order assigned to Robot " << robotID << ".\n";
            
            // Free robot anyway
            slots[robotSlot].status = "Available";
            slots[robotSlot].assignedOrderId = -1;
            slots[robotSlot].totalTaskDone++;
            
            saveToCSV(ROBOTS_FILE);
            
            return false;
        }

        // Move completed order to completedQueue
        completedQueue.enqueue(doneOrder);

        // Reset robot information
        slots[robotSlot].status = "Available";
        slots[robotSlot].assignedOrderId = -1;
        slots[robotSlot].totalTaskDone++;

        cout << "\n[OK] Robot " << robotID
             << " has completed its task and is now available.\n";

        //Save updated robot data
        saveToCSV(ROBOTS_FILE);

        // Auto-assign next order if waiting queue still has orders
        if(!waitingQueue.isEmpty()){
            
            cout << "\n[Reminder] Checking waiting queue for next order..." << endl;
            assignNextFromWaiting();
        }

        return true;
    }

    // Toggle Maintenance mode
    bool toggleMaintenance(int robotID) {
        
        //Search robot
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].robotID == robotID) {
                
                //Prevent maintenance if robot is busy
                if (slots[index].status == "Busy") {
                    cout << "\n[Reminder] Robot " << robotID
                         << " is still busy. Complete its task first.\n";
                    return false;
                }
                
                //Change available to Maintenance
                if (slots[index].status == "Available") {
                    slots[index].status = "Maintenance";
                    cout << "\n[OK] Robot " << robotID << " has been sent to maintenance.\n";
                }

                //Chg Maintenance to Available
                else{
                    slots[index].status = "Available";
                    cout << "\n[OK] Robot " << robotID << " has been restored and is now available.\n";
                } 
                
                saveToCSV(ROBOTS_FILE);
                
                return true;
            }
        }
        
        //Robot not found
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
        
        //Display all robot details
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
        
        //No available robots
        if (!found)
            cout << "  No robots are available right now." << endl;
    }
 
    // Load robots from a CSV file
    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        
        //Chk if file can be opened
        if (!file.is_open()) {
            cout << "\n[ERROR] Could not open file: " << filename << endl;
            return;
        }
 
        string line;
        
        //Skip CSV header
        getline(file, line); // Skip header row
 
        int loaded = 0;
        while (getline(file, line) && !IsFull()) {
            
            // Remove '\r' if exists
            if (!line.empty() && line.back() == '\r') 
            line.pop_back();
 
            stringstream ss(line);
            
            string idStr, statusStr;
            
            getline(ss, idStr,     ',');
            getline(ss, statusStr, ',');
 
            //Skip invalid row
            if (idStr.empty()) 
                continue;
 
            //Standardize status text format
            if (!statusStr.empty()) {
                statusStr[0] = toupper(statusStr[0]);
                
                for (int i = 1; i < (int)statusStr.size(); i++)
                    statusStr[i] = tolower(statusStr[i]);
            }

            //Default invalid status to Available
            if (statusStr != "Available" && statusStr != "Maintenance")
                statusStr = "Available";
 
            Robot r;
            
            r.robotID = stoi(idStr);
            r.status  = statusStr;
 
            //Insert robot directly into queue
            rear = (rear + 1) % cap;
            slots[rear] = r;
            
            count++;
            loaded++;
        }
        
        file.close();
    }

    //Save robots data into CSV file
    void saveToCSV(const string& filename) {
        ofstream file(filename);
        
        //Chk file opening
        if (!file.is_open()) {
            cout << "\n[ERROR] Could not open file for writing: " << filename << endl;
            return;
        }
        
        //CSV header
        file << "robotID,status\n";
        
        //Save all robots
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            
            //Busy robots are saved as Available
            string savedStatus = (slots[index].status == "Busy") ? "Available" : slots[index].status;
            
            file << slots[index].robotID << "," << savedStatus << "\n";
        }
        
        file.close();
    }

    //Add new robot into queue
    void addRobot() {
        
        //Prevent overflow
        if (IsFull()) {
            cout << "\n[ERROR] Cannot add robot — queue is at full capacity (" << cap << ")." << endl;
            return;
        }
        
        //Find highest robot ID
        int maxID = 0;

        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].robotID > maxID)
                maxID = slots[index].robotID;
        }

        //Create new robot
        Robot r;
        
        r.robotID = maxID + 1;
        r.status  = "Available";
        
        //Insert into queue
        rear = (rear + 1) % cap;
        slots[rear] = r;
        
        count++;

        processingQueue.resize(count);
        
        cout << "\n[OK] Robot " << r.robotID << " added and set to Available." << endl;
        
        saveToCSV(ROBOTS_FILE);
    }

    //Delete robot from queue
    void deleteRobot(int robotID) {
        int found = -1;
        
        //Search robot
        for (int i = 0; i < count; i++) {
            int index = (front + i) % cap;
            if (slots[index].robotID == robotID) {
                
                found = i;
                break;
            }
        }
        
        //Robot not found
        if (found == -1) {
            cout << "\n[ERROR] Robot " << robotID << " not found." << endl;
            return;
        }
        
        int index = (front + found) % cap;
        
        //Prevent deletion if robot is busy
        if (slots[index].status == "Busy") {
            cout << "\n[Reminder] Robot " << robotID
                 << " is currently busy. Complete its task before deleting." << endl;
            return;
        }

        //Create new queue without deleted robot
        Robot* newSlots = new Robot[cap];

        int newCount = 0;

        for (int i = 0; i < count; i++) {
            int idx = (front + i) % cap;
            if (slots[idx].robotID != robotID)
                newSlots[newCount++] = slots[idx];
        }

        //Replace old queue
        delete[] slots;
        
        slots  = newSlots;
        
        front  = 0;
        rear   = (newCount == 0) ? -1 : newCount - 1;
        
        count  = newCount;
        
        //Adjust round-robin pointer
        currentTurn = (count == 0) ? 0 : currentTurn % count;
        
        cout << "\n[OK] Robot " << robotID << " has been deleted." << endl;
        
        saveToCSV(ROBOTS_FILE);  
    }

    void clear(){
        front = 0;
        rear = -1;
        count = 0;
        currentTurn = 0;
    }
};

//External declaration of robot queue
extern CircularQueue robotQueue;

//Function declaration
void runTask2();