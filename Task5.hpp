#ifndef TASK5_HPP
#define TASK5_HPP

/*****************************************************************
 * TASK 5: WAREHOUSE LAYOUT AND NAVIGATION MODULE (OPTIONAL)
 * ---------------------------------------------------------------
 *****************************************************************/

#include <string>

namespace Task5 {

    // ============================================================
    // Node - building block for LinkedList, Queue, and Stack
    // ============================================================
    struct Node {
        int data;
        Node* next;
    };

    // ============================================================
    // SELF-MADE LINKED LIST
    // WHY: We need a dynamic list of neighbor IDs per location.
    //      Cannot use std::vector or std::list (assignment rule).
    // ============================================================
    class LinkedList {
    public:
        Node* head;
        int   size;

        LinkedList();
        ~LinkedList();
        void add(int value);
        bool contains(int value);
        void display();        // prints "a -> b -> c"
    };

    // ============================================================
    // QUEUE (FIFO)
    // WHY: BFS requires FIFO order to guarantee SHORTEST path
    //      in an unweighted graph.
    // ============================================================
    class Queue {
    public:
        Node* front;
        Node* rear;

        Queue();
        ~Queue();
        void enqueue(int value);
        int dequeue();        // returns -1 if empty
        bool isEmpty();
    };

    // ============================================================
    // SELF-MADE STACK (LIFO)
    // WHY: 1) DFS uses LIFO to dive deep before backtracking
    //      2) Path reconstruction: we walk parents backwards
    //         (end -> start) and push to stack, then pop to
    //         get the path in correct order (start -> end).
    // ============================================================
    class Stack {
    public:
        Node* top;

        Stack();
        ~Stack();
        void push(int value);
        int pop();            // returns -1 if empty
        bool isEmpty();
    };

    // ============================================================
    // LOCATION - one node in the warehouse graph
    // type is "Zone", "Aisle", or "Shelf"
    // ============================================================
    struct Location {
        int id;
        std::string name;
        std::string type;
        LinkedList  neighbors;
    };

    // ============================================================
    // WAREHOUSE - the graph of all locations
    // ============================================================
    class Warehouse {
    private:
        static const int MAX_LOCATIONS = 100;
        Location locations[MAX_LOCATIONS];
        int locationCount;

    public:
        Warehouse();

        // ---- Layout management ----
        void addLocation(int id, const std::string& name, const std::string& type);
        void connect(int id1, int id2);
        int findIndex(int id) const;        // returns -1 if not found
        int getLocationCount() const;

        // ---- Display ----
        void displayLayout();                // textual list of all nodes/edges
        void displayVisualMap();             

        // ---- Navigation ----
        void findPath(int startId, int endId);     // BFS, prints route
        void traverseAll(int startId);             // DFS, prints visit order

        // ---- Integration with Task 3 (Robot Navigation) ----
        // Returns the BFS shortest path as a Stack (top = start, bottom = end).
        // Pop the stack in order to drive the robot step-by-step.
        // Returns an empty stack if no path exists.
        Stack* getPathBetween(int startId, int endId);

        // Helper used by other tasks: get the human-readable name of a location
        std::string getLocationName(int id) const;
    };

    // Entry point called from main.cpp
    void runTask5();

}

#endif
