/*****************************************************************
 * TASK 5: WAREHOUSE LAYOUT AND NAVIGATION MODULE (OPTIONAL)
 * Implementation file (Task5.cpp)
 *
 * Implements:
 *   - LinkedList, Queue, Stack (self-made containers)
 *   - Warehouse class (graph with BFS + DFS)
 *   - Interactive menu via runTask5()
 *****************************************************************/

#include "Task5.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

namespace Task5 {

    // ============================================================
    // LINKED LIST IMPLEMENTATION
    // ============================================================
    LinkedList::LinkedList() {
        head = nullptr;
        size = 0;
    }

    LinkedList::~LinkedList() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        head = nullptr;
    }

    void LinkedList::add(int value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    bool LinkedList::contains(int value) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data == value) return true;
            temp = temp->next;
        }
        return false;
    }

    void LinkedList::display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data;
            if (temp->next != nullptr) cout << " -> ";
            temp = temp->next;
        }
    }

    // ============================================================
    // QUEUE IMPLEMENTATION
    // ============================================================
    Queue::Queue() {
        front = nullptr;
        rear  = nullptr;
    }

    Queue::~Queue() {
        while (!isEmpty()) dequeue();
    }

    void Queue::enqueue(int value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = nullptr;

        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    int Queue::dequeue() {
        if (front == nullptr) return -1;
        Node* temp = front;
        int value  = temp->data;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return value;
    }

    bool Queue::isEmpty() {
        return front == nullptr;
    }

    // ============================================================
    // STACK IMPLEMENTATION
    // ============================================================
    Stack::Stack() {
        top = nullptr;
    }

    Stack::~Stack() {
        while (!isEmpty()) pop();
    }

    void Stack::push(int value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = top;
        top = newNode;
    }

    int Stack::pop() {
        if (top == nullptr) return -1;
        Node* temp = top;
        int value  = temp->data;
        top = top->next;
        delete temp;
        return value;
    }

    bool Stack::isEmpty() {
        return top == nullptr;
    }

    // ============================================================
    // WAREHOUSE CLASS IMPLEMENTATION
    // ============================================================
    Warehouse::Warehouse() {
        locationCount = 0;
    }

    void Warehouse::addLocation(int id, const string& name, const string& type) {
        if (locationCount >= MAX_LOCATIONS) {
            cout << "! Warehouse is full. Cannot add more locations." << endl;
            return;
        }
        if (findIndex(id) != -1) {
            cout << "! Location ID " << id << " already exists." << endl;
            return;
        }
        locations[locationCount].id   = id;
        locations[locationCount].name = name;
        locations[locationCount].type = type;
        locationCount++;
    }

    void Warehouse::connect(int id1, int id2) {
        int i1 = findIndex(id1);
        int i2 = findIndex(id2);
        if (i1 == -1 || i2 == -1) {
            cout << "! Cannot connect: invalid ID." << endl;
            return;
        }
        // Undirected edge: add neighbor on both sides
        if (!locations[i1].neighbors.contains(id2))
            locations[i1].neighbors.add(id2);
        if (!locations[i2].neighbors.contains(id1))
            locations[i2].neighbors.add(id1);
    }

    int Warehouse::findIndex(int id) const {
        for (int i = 0; i < locationCount; i++) {
            if (locations[i].id == id) return i;
        }
        return -1;
    }

    int Warehouse::getLocationCount() const {
        return locationCount;
    }

    string Warehouse::getLocationName(int id) const {
        int idx = findIndex(id);
        if (idx == -1) return "Unknown";
        return locations[idx].name;
    }

    // ----------------------------------------------------
    // Textual layout: list every location and its neighbors
    // ----------------------------------------------------
    void Warehouse::displayLayout() {
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "   WAREHOUSE LAYOUT (Adjacency List)" << endl;
        cout << string(40, '=') << endl;
        for (int i = 0; i < locationCount; i++) {
            cout << "[" << locations[i].id << "] "
                 << locations[i].type << ": "
                 << locations[i].name
                 << "  --> Connected to: ";
            locations[i].neighbors.display();
            cout << endl;
        }
        cout << string(40, '=') << endl;
    }

    // ----------------------------------------------------
    // Visual ASCII map (hierarchical view for presentation)
    // ----------------------------------------------------
    void Warehouse::displayVisualMap() {
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "WAREHOUSE MAP (Visual)" << endl;
        cout << string(40, '=') << endl;

        // Print every Zone, then the Stations, Aisles and Shelves
        // directly connected to it.
        for (int i = 0; i < locationCount; i++) {
            if (locations[i].type != "Zone") continue;
            cout << "[Zone] " << locations[i].name << endl;

            Node* n = locations[i].neighbors.head;
            while (n != nullptr) {
                int idx = findIndex(n->data);
                if (idx != -1) {
                    const string& t = locations[idx].type;
                    if (t == "Station") {
                        cout << " |-- [Station] " << locations[idx].name << endl;
                    } else if (t == "Shelf") {
                        cout << " |-- [Shelf]   " << locations[idx].name << endl;
                    } else if (t == "Aisle") {
                        cout << " |-- [Aisle]   " << locations[idx].name << endl;
                        // walk that aisle's neighbors -> find Shelves
                        Node* sn = locations[idx].neighbors.head;
                        while (sn != nullptr) {
                            int sidx = findIndex(sn->data);
                            if (sidx != -1 && locations[sidx].type == "Shelf") {
                                cout << "   |       |-- [Shelf] "
                                     << locations[sidx].name << endl;
                            }
                            sn = sn->next;
                        }
                    }
                }
                n = n->next;
            }
            cout << endl;
        }
        cout << string(40, '=') << endl;
    }

    // ----------------------------------------------------
    // BFS - Shortest path between two locations
    // ----------------------------------------------------
    void Warehouse::findPath(int startId, int endId) {
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "  PATH FROM " << startId << " TO " << endId << endl;
        cout << string(40, '=') << endl;

        Stack* path = getPathBetween(startId, endId);
        if (path == nullptr || path->isEmpty()) {
            cout << "! No path found between these locations." << endl;
            if (path != nullptr) delete path;
            return;
        }

        cout << "Route: ";
        bool first = true;
        while (!path->isEmpty()) {
            int id = path->pop();
            if (!first) cout << " -> ";
            cout << getLocationName(id);
            first = false;
        }
        cout << endl;
        delete path;
    }

    // ----------------------------------------------------
    // Returns a Stack of location IDs (top = start, bottom = end).
    // The robot pops from the stack to follow the path step by step.
    // ----------------------------------------------------
    Stack* Warehouse::getPathBetween(int startId, int endId) {
        Stack* result = new Stack();

        if (findIndex(startId) == -1 || findIndex(endId) == -1) {
            return result;  // empty stack signals "no path"
        }

        // BFS
        bool visited[MAX_LOCATIONS];
        int  parent [MAX_LOCATIONS];
        for (int i = 0; i < MAX_LOCATIONS; i++) {
            visited[i] = false;
            parent [i] = -1;
        }

        Queue q;
        q.enqueue(startId);
        visited[findIndex(startId)] = true;

        bool found = false;
        while (!q.isEmpty()) {
            int current = q.dequeue();
            if (current == endId) { found = true; break; }

            int idx = findIndex(current);
            Node* temp = locations[idx].neighbors.head;
            while (temp != nullptr) {
                int neighborIdx = findIndex(temp->data);
                if (neighborIdx != -1 && !visited[neighborIdx]) {
                    visited[neighborIdx] = true;
                    parent [neighborIdx] = current;
                    q.enqueue(temp->data);
                }
                temp = temp->next;
            }
        }

        if (!found) return result;

        // Walk parents end -> start, push each onto the stack
        // so popping yields the correct order start -> end.
        int curr = endId;
        while (curr != -1) {
            result->push(curr);
            int idx = findIndex(curr);
            curr = (idx != -1) ? parent[idx] : -1;
        }
        return result;
    }

    // ----------------------------------------------------
    // DFS - Traverse every reachable warehouse section
    // ----------------------------------------------------
    void Warehouse::traverseAll(int startId) {
        cout << endl;
        cout << string(40, '=') << endl;
        cout << "  FULL WAREHOUSE TRAVERSAL (DFS)" << endl;
        cout << string(40, '=') << endl;

        if (findIndex(startId) == -1) {
            cout << "! Invalid starting location ID." << endl;
            return;
        }

        bool visited[MAX_LOCATIONS];
        for (int i = 0; i < MAX_LOCATIONS; i++) visited[i] = false;

        Stack s;
        s.push(startId);

        cout << "Order Visited:" << endl;
        int step = 1;
        while (!s.isEmpty()) {
            int current = s.pop();
            int idx     = findIndex(current);
            if (idx == -1 || visited[idx]) continue;
            visited[idx] = true;

            cout << "  " << step++ << ". "
                 << locations[idx].type << " -> "
                 << locations[idx].name << endl;

            // Push neighbors so the first one ends up on top (visited next)
            Node* temp = locations[idx].neighbors.head;
            while (temp != nullptr) {
                int neighborIdx = findIndex(temp->data);
                if (neighborIdx != -1 && !visited[neighborIdx]) {
                    s.push(temp->data);
                }
                temp = temp->next;
            }
        }
    }

    // ============================================================
    // ID SCHEME (so zones, shelves and stations never collide)
    //   Zone  letter X  -> id = 100 + (X - 'A')   [A=100 ... K=110]
    //   Shelf number N  -> id = N                 [1 ... 15]
    //   Station letter X-> id = 200 + (X - 'A')   [A=200 ... L=211]
    // ============================================================
    static int zoneId(char z)    { return 100 + (z - 'A'); }   // A=100 ... K=110
    static int stationId(char s) { return 200 + (s - 'A'); }   // A=200 ... L=211
    // (shelves just use their own number 1-15 as the ID)

    // ============================================================
    // Build the warehouse graph from the CSV files.
    // ============================================================
    void buildWarehouseFromCSV(Warehouse& wh) {
        string line;

        // ---- 1) ZONES (one letter per line) ----
        ifstream zoneFile("zones.csv");
        getline(zoneFile, line);                 // skip header
        int prevZone = -1;
        while (getline(zoneFile, line)) {
            if (line.empty()) continue;
            char z = line[0];
            wh.addLocation(zoneId(z), string("Zone-") + z, "Zone");
            if (prevZone != -1) wh.connect(prevZone, zoneId(z));  // chain A-B-C-...
            prevZone = zoneId(z);
        }

        // ---- 2) SHELVES (items.csv: id,name,zone,shelf) ----
        ifstream itemFile("items.csv");
        getline(itemFile, line);                 // skip header
        while (getline(itemFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string id, name, zoneStr, shelfStr;
            getline(ss, id,    ',');
            getline(ss, name,  ',');
            getline(ss, zoneStr, ',');
            getline(ss, shelfStr, ',');

            char z  = zoneStr[0];
            int  sn = stoi(shelfStr);
            wh.addLocation(sn, "Shelf-" + shelfStr + " (Zone-" + z + ")", "Shelf");
            wh.connect(zoneId(z), sn);           // link shelf to its zone
        }

        // ---- 3) PACKING STATIONS (one letter per line) ----
        ifstream stationFile("stations.csv");
        getline(stationFile, line);              // skip header
        while (getline(stationFile, line)) {
            if (line.empty()) continue;
            char s = line[0];
            wh.addLocation(stationId(s), string("PackingStation-") + s, "Station");
            if (wh.findIndex(zoneId(s)) != -1)   // connect only if that zone exists
                wh.connect(stationId(s), zoneId(s));
        }
    }

    // ============================================================
    // Helper: read an integer safely (handles non-numeric input)
    // ============================================================
    static bool readInt(int& out) {
        cin >> out;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return false;
        }
        return true;
    }

    // ============================================================
    // Entry point - interactive menu
    // ============================================================
    void runTask5() {
        Warehouse wh;
        buildWarehouseFromCSV(wh);

        int choice = 0;
        do {
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "  Task 5: Warehouse Navigation Menu" << endl;
            cout << string(40, '=') << endl;
            cout << "1. Display warehouse layout (adjacency list)" << endl;
            cout << "2. Display warehouse map (visual hierarchy)" << endl;
            cout << "3. Find shortest path between two locations" << endl;
            cout << "4. Traverse all warehouse sections (DFS)"   << endl;
            cout << "5. Run full demo (preset paths + traversal)" << endl;
            cout << "6. Back to main menu" << endl;
            cout << "Enter your choice: ";

            if (!readInt(choice)) {
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "Invalid input. Please enter 1-6." << endl;
                cout << string(40, '=') << endl;
                continue;
            }

            switch (choice) {
                case 1:
                    wh.displayLayout();
                    break;

                case 2:
                    wh.displayVisualMap();
                    break;

                case 3: {
                    int startId, endId;
                    cout << "Enter START location ID: ";
                    if (!readInt(startId)) { cout << "! Invalid input." << endl; break; }
                    cout << "Enter END   location ID: ";
                    if (!readInt(endId))   { cout << "! Invalid input." << endl; break; }
                    wh.findPath(startId, endId);
                    break;
                }

                case 4: {
                    int startId;
                    cout << "Enter starting location ID: ";
                    if (!readInt(startId)) { cout << "! Invalid input." << endl; break; }
                    wh.traverseAll(startId);
                    break;
                }

                case 5:
                    wh.displayVisualMap();
                    wh.displayLayout();
                    wh.findPath(200, 8);     // PackingStation-A -> Shelf-8 (Zone C)
                    wh.findPath(100, 12);    // Zone-A -> Shelf-12 (Zone D)
                    wh.findPath(1, 15);      // Shelf-1 (Zone A) -> Shelf-15 (Zone E)
                    wh.traverseAll(200);     // DFS from PackingStation-A
                    break;

                case 6:
                    cout << "Returning to main menu..." << endl;
                    break;

                default:
                    cout << endl;
                    cout << string(40, '=') << endl;
                    cout << "Invalid choice. Please enter 1-6." << endl;
                    cout << string(40, '=') << endl;
            }
        } while (choice != 6);
    }
}
