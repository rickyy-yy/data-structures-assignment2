#pragma once
#include "Task1.hpp"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

//Stores information about a delivery robot
struct Robot{
    int robotID;    //unique identifier for robot
    string status;  //Robot status: Availble, Busy and Mainenance
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
    int cap, front, rear, count;

    //Constructor
    //Initialize circular queue with given capacity
    CircularQueue(int cq) : cap(cq), front(0), rear(-1), count(0){
        slots = new Robot [cap];
    }
    
    //Destructor
    //Release allocated memory
    ~CircularQueue(){
        delete [] slots;
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

    //Print out the robot that currently in the queue
    void display(){
        
        //Chk whether queue is empty
        if (IsEmpty()){
            cout << "\n[ERROR] No robots are currently in the queue now." << endl;
            return;
        }
        
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Robots in Circular Queue:" << endl;
        cout << string(40, '=') << endl << endl;
        
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
};

//Handles robot assignment and task management
struct RobotAssignmentSystem {
    Robot* robots;  //dynamic array storing all robots
    int total;  //total number of robots
    int nextTurn;   //used for round-robin assignment

    //Constructor
    //Create robot system with n robots
    RobotAssignmentSystem(int n) : total(n), nextTurn(0) {
        robots = new Robot[n];
        
        //Assign robot id
        for (int i = 0; i < n; i++)
            robots[i].robotID = i + 1;
    }
    
    //Destructor
    ~RobotAssignmentSystem() { delete[] robots; }

    //Assign available robot to an order
    //Uses round-robin scheduling
    int assignRobot(Order* order) {
        for (int i = 0; i < total; i++) {
            int index  = nextTurn;
            
            //Move to next robot for future assignment
            nextTurn = (nextTurn + 1) % total;  

            //Chk for available robot
            if (robots[index].status == "Available") {
                //Update robot information
                robots[index].status = "Busy";
                robots[index].assignedOrderId = order->orderID;
                robots[index].totalTaskDone++;

                //Update order status
                order->robotID = robots[index].robotID;
                order->status = "Processing";

                cout << endl;
                cout << string(40, '=') << endl;
                cout << "[Reminder] Order " << order->orderID
                     << " is now being processed by Robot "
                     << robots[index].robotID << "." << endl;
                cout << string(40, '=') << endl;
                return robots[index].robotID;
            } 
        }

        //No available robot found
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "[Reminder] All robots are curently busy. Please try again later." << endl;
        cout << string(40, '=') << endl;
        return -1;
    }

    //Mark robot task as completed
    bool completeTask(int id) {
        //Search robot by ID
        for (int i = 0; i < total; i++) {
            if (robots[i].robotID == id) {
                
                //Robot must be busy before completion
                if (robots[i].status != "Busy") {
                    cout << "\n[Reminder] Robot " << id << " is not currently busy.\n";
                    return false;
                }
                
                //Reset robot status
                robots[i].status = "Available";
                robots[i].assignedOrderId = -1;
                cout << "\n[Reminder] Robot " << id << " has completed its task and is now available.\n";
                return true;
            }
        }
        
        //Robot not found
        cout << "\n[ERROR] Robot " << id << " not found.\n";
        return false;
    }

    //Put robot under maintenance
    bool setMaintenance(int id) {
        for (int i = 0; i < total; i++) {
            
            //Busy robot cannot enter maintenance
            if (robots[i].robotID == id) {
                if (robots[i].status == "Busy") {
                    cout << "\n[Reminder] Robot " << id << " is still busy. Please complete its current task first.\n";
                    return false;
                }
                
                //Update robot status
                robots[i].status = "Maintenance";
                robots[i].assignedOrderId = -1;
                cout << "\n[Reminder] Robot " << id << " has been sent to maintenance.\n";
                return true;
            }
        }

        //Robot not found
        cout << "\n[ERROR] Robot " << id << " not found.\n";
        return false;
    }

    //Restore robot from maintenance mode
    bool restoreRobot(int id) {
        for (int i = 0; i < total; i++) {
            if (robots[i].robotID == id) {
                
                //Robot must currently be under maintenance
                if (robots[i].status != "Maintenance") {
                    cout << "\n[Reminder] Robot " << id << " is not currently under maintenance.\n";
                    return false;
                }
                
                //Restore robot status
                robots[i].status = "Available";
                cout << "\n[Reminder] Robot " << id << " has been restored and is now available.\n";
                return true;
            }
        }
        
        //Robot not found
        cout << "\n[ERROR] Robot " << id << " not found.\n";
        return false;
    }

    //Display status of all robots
    void displayAll() {
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Robot Status Overview:" << endl;
        cout << string(40, '=') << endl << endl;
        
        //Table header
        cout << left
             << setw(10) << "Robot ID" << "| "
             << setw(14) << "Status"   << "| "
             << setw(12) << "Order ID" << "| "
             << "Tasks Done" << endl;
        
        cout << string(50, '-') << endl;
        
        //Display each robot
        for (int i = 0; i < total; i++) {
            string ord = (robots[i].assignedOrderId == -1) ? "None" : to_string(robots[i].assignedOrderId);
            cout << left
                 << setw(10) << robots[i].robotID << "| "
                 << setw(14) << robots[i].status  << "| "
                 << setw(12) << ord               << "| "
                 << robots[i].totalTaskDone << endl;
        }
    }

    //Show all available robots
    void displayAvailable() {
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Available Robots:" << endl;
        cout << string(40, '=') << endl;
        bool found = false;
        
        //Search for available robots
        for (int i = 0; i < total; i++) {
            if (robots[i].status == "Available") {
                cout << "  -> Robot " << robots[i].robotID
                     << " (tasks completed: " << robots[i].totalTaskDone << ")" << endl;
                found = true;
            }
        }
        
        //No available robots
        if (!found) cout << "  No robots are available right now." << endl;
    }
};

void runTask2();

