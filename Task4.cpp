#include "Task4.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;


namespace Task1 {

    
    Order* createOrder(int& nextOrderID) {
        Order* newOrder  = new Order();
        newOrder->orderID = nextOrderID++;

        cout << endl;
        cout << string(40, '=') << endl;
        cout << "Recording New Order" << endl;
        cout << string(40, '=') << endl;
        cout << "Order ID: " << newOrder->orderID << endl;

        cout << "Enter Item ID: ";
        while (!(cin >> newOrder->itemID)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter Item ID: ";
        }

        cout << "Enter Shelf Number: ";
        while (!(cin >> newOrder->shelfNumber)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter Shelf Number: ";
        }

        cout << "Enter Packing Station: ";
        cin  >> newOrder->packingStation;

        cout << "Enter Zone: ";
        cin  >> newOrder->zone;

        return newOrder;
    }

    void runTask1() {
        UnlimitedQueue  pendingQueue;
        ProcessingQueue processingQueue(5);   
        UnlimitedQueue  completedQueue;
        int nextRobotID = 1;
        int nextOrderID = 1;
        int choice      = 0;

        do {
            cout << endl;
            cout << string(40, '=') << endl;
            cout << "Task 1: Order Management Module" << endl;
            cout << string(40, '=') << endl;
            cout << "1. Record a new order"            << endl;
            cout << "2. Process an order"              << endl;
            cout << "3. Display all pending orders"    << endl;
            cout << "4. Display all processing orders" << endl;
            cout << "5. Display all completed orders"  << endl;
            cout << "6. Return to main menu"           << endl;
            cout << "Enter your choice (1-6): ";
            cin  >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl;
                cout << string(40, '=') << endl;
                cout << "! Invalid input. Please enter a number between 1 and 6." << endl;
                cout << string(40, '=') << endl;
            } else {
                switch (choice) {
                case 1:
                    pendingQueue.enqueue(createOrder(nextOrderID));
                    cout << endl;
                    cout << string(40, '=') << endl;
                    cout << "! Order added to queue." << endl;
                    cout << string(40, '=') << endl;
                    break;

                case 2:
                    if (pendingQueue.isEmpty()) {
                        cout << endl;
                        cout << string(40, '=') << endl;
                        cout << "! No orders to process." << endl;
                        cout << string(40, '=') << endl;
                    } else if (processingQueue.isFull()) {
                        cout << endl;
                        cout << string(40, '=') << endl;
                        cout << "! All robots are busy. Cannot process more orders." << endl;
                        cout << string(40, '=') << endl;
                    } else {
                        Order* order = pendingQueue.dequeue();
                        order->assignRobot(nextRobotID++);
                        order->inProcess();
                        processingQueue.enqueue(order);
                        cout << endl;
                        cout << string(40, '=') << endl;
                        cout << "! Order " << order->orderID
                             << " is now being processed by Robot "
                             << order->robotID << "." << endl;
                        cout << string(40, '=') << endl;
                    }
                    break;

                case 3:
                    pendingQueue.displayAll();
                    break;

                case 4:
                    processingQueue.displayAll();
                    break;

                case 5:
                    completedQueue.displayAll();
                    break;

                case 6:
                    cout << "Returning to main menu..." << endl;
                    break;

                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
        } while (choice != 6);
    }

} 



namespace Task4 {

    
    ItemManager::ItemManager()  { root = nullptr; }
    ItemManager::~ItemManager() { destroyTree(root); }

    
    void ItemManager::destroyTree(BSTNode* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    
    bool ItemManager::isDuplicate(BSTNode* node, int id) {
        if (node == nullptr)          return false;
        if (id == node->data.itemID)  return true;
        if (id <  node->data.itemID)  return isDuplicate(node->left,  id);
        return                               isDuplicate(node->right, id);
    }

    
    BSTNode* ItemManager::insertHelper(BSTNode* node, Item item) {
        if (node == nullptr) return new BSTNode(item);  

        if (item.itemID < node->data.itemID)
            node->left  = insertHelper(node->left,  item);
        else
            node->right = insertHelper(node->right, item);

        return node;
    }

    
    void ItemManager::insertItem(int id, const string& name, const string& location) {
        if (id <= 0) {
            cout << "  [ERROR] Item ID must be a positive integer.\n";
            return;
        }
        if (isDuplicate(root, id)) {
            cout << "  [ERROR] Item ID " << id
                 << " already exists. Use 'Update' to modify it.\n";
            return;
        }
        Item newItem = { id, name, location };
        root = insertHelper(root, newItem);
        cout << "  [SUCCESS] Item inserted -> ID: " << id
             << " | " << name << " @ " << location << "\n";
    }

    
    BSTNode* ItemManager::searchByID(BSTNode* node, int id) {
        if (node == nullptr)          return nullptr;
        if (id == node->data.itemID)  return node;
        if (id <  node->data.itemID)  return searchByID(node->left,  id);
        return                               searchByID(node->right, id);
    }

    
    void ItemManager::searchByID(int id) {
        BSTNode* result = searchByID(root, id);
        if (result == nullptr) {
            cout << "  [NOT FOUND] No item with ID " << id << ".\n";
        } else {
            cout << "  [FOUND]\n";
            cout << "  +---------+----------------------+-----------------------------+\n";
            cout << "  | " << left << setw(7)  << "Item ID"
                 << " | " << setw(20) << "Item Name"
                 << " | " << setw(27) << "Location" << " |\n";
            cout << "  +---------+----------------------+-----------------------------+\n";
            cout << "  | " << left << setw(7)  << result->data.itemID
                 << " | " << setw(20) << result->data.itemName
                 << " | " << setw(27) << result->data.location << " |\n";
            cout << "  +---------+----------------------+-----------------------------+\n";
        }
    }

    
    void ItemManager::searchByName(BSTNode* node, const string& keyword, bool& found) {
        if (node == nullptr) return;
        searchByName(node->left, keyword, found);   

        string nodeName = node->data.itemName;
        string key      = keyword;
        transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::tolower);
        transform(key.begin(),      key.end(),      key.begin(),      ::tolower);

        if (nodeName.find(key) != string::npos) {
            if (!found) {
                
                cout << "  +---------+----------------------+-----------------------------+\n";
                cout << "  | " << left << setw(7)  << "Item ID"
                     << " | " << setw(20) << "Item Name"
                     << " | " << setw(27) << "Location" << " |\n";
                cout << "  +---------+----------------------+-----------------------------+\n";
                found = true;
            }
            cout << "  | " << left << setw(7)  << node->data.itemID
                 << " | " << setw(20) << node->data.itemName
                 << " | " << setw(27) << node->data.location << " |\n";
        }
        searchByName(node->right, keyword, found);  
    }

    
    void ItemManager::searchByName(const string& keyword) {
        bool found = false;
        cout << "  Searching for items with name containing: \"" << keyword << "\"\n";
        searchByName(root, keyword, found);
        if (found) {
            cout << "  +---------+----------------------+-----------------------------+\n";
        } else {
            cout << "  [NOT FOUND] No items match the keyword \"" << keyword << "\".\n";
        }
    }

    
    void ItemManager::updateItem(int id, const string& newName, const string& newLocation) {
        BSTNode* target = searchByID(root, id);
        if (target == nullptr) {
            cout << "  [ERROR] Item ID " << id << " not found.\n";
        } else {
            target->data.itemName = newName;
            target->data.location = newLocation;
            cout << "  [UPDATED] ID " << id
                 << " -> Name: " << newName
                 << " | Location: " << newLocation << "\n";
        }
    }

    
    BSTNode* ItemManager::findMin(BSTNode* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    
    BSTNode* ItemManager::deleteHelper(BSTNode* node, int id) {
        if (node == nullptr) return nullptr;

        if (id < node->data.itemID) {
            node->left  = deleteHelper(node->left,  id);
        } else if (id > node->data.itemID) {
            node->right = deleteHelper(node->right, id);
        } else {
            
            if (node->left == nullptr) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            
            BSTNode* successor   = findMin(node->right);
            node->data           = successor->data;
            node->right          = deleteHelper(node->right, successor->data.itemID);
        }
        return node;
    }

    
    void ItemManager::deleteItem(int id) {
        if (searchByID(root, id) == nullptr) {
            cout << "  [ERROR] Item ID " << id << " not found.\n";
            return;
        }
        root = deleteHelper(root, id);
        cout << "  [DELETED] Item ID " << id << " removed from database.\n";
    }

    
    void ItemManager::inOrderDisplay(BSTNode* node, int& count) {
        if (node == nullptr) return;
        inOrderDisplay(node->left, count);
        count++;
        cout << "  | " << left << setw(7)  << node->data.itemID
             << " | " << setw(20) << node->data.itemName
             << " | " << setw(27) << node->data.location << " |\n";
        inOrderDisplay(node->right, count);
    }

    
    void ItemManager::displayAll() {
        if (root == nullptr) {
            cout << "  [INFO] Item database is empty.\n";
            return;
        }
        cout << "\n  ===== WAREHOUSE ITEM DATABASE (Sorted by ID) =====\n";
        cout << "  +---------+----------------------+-----------------------------+\n";
        cout << "  | " << left << setw(7)  << "Item ID"
             << " | " << setw(20) << "Item Name"
             << " | " << setw(27) << "Location" << " |\n";
        cout << "  +---------+----------------------+-----------------------------+\n";
        int count = 0;
        inOrderDisplay(root, count);
        cout << "  +---------+----------------------+-----------------------------+\n";
        cout << "  Total items: " << count << "\n\n";
    }

    
    void ItemManager::showMenu() {
        cout << string(40, '=') << endl;
        cout << "Task 4: Item search and Management Module" << endl; 
        cout << string(40, '=') << endl;        
        cout << "1. Insert New Item" << endl;
        cout << "2. Search Item by ID" << endl;
        cout << "3. Search Item by Name" << endl;
        cout << "4. Update Item Details" << endl;
        cout << "5. Delete Item" << endl;
        cout << "6. Display All Items (Sorted by ID)" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << "  Enter your choice: ";
    }

    
    void ItemManager::run() {
        int choice = -1;

        
        cout << "\n  [INFO] Loading sample warehouse data...\n";
        insertItem(101, "Laptop",          "ZoneA-Aisle1-Shelf2");
        insertItem(203, "USB Hub",         "ZoneA-Aisle3-Shelf1");
        insertItem(155, "Wireless Mouse",  "ZoneB-Aisle1-Shelf4");
        insertItem(340, "Keyboard",        "ZoneB-Aisle2-Shelf2");
        insertItem(89,  "HDMI Cable",      "ZoneC-Aisle1-Shelf1");
        insertItem(278, "Webcam",          "ZoneA-Aisle2-Shelf3");
        cout << "  [INFO] Sample data loaded.\n";

        while (choice != 0) {
            showMenu();

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  [ERROR] Invalid input. Please enter a number.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n";

            if (choice == 1) {
                
                int    id;
                string name, location;
                cout << "  Enter Item ID       : ";
                if (!(cin >> id)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "  [ERROR] Invalid ID.\n"; continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Enter Item Name     : ";  getline(cin, name);
                cout << "  Enter Item Location : ";  getline(cin, location);
                insertItem(id, name, location);

            } else if (choice == 2) {
                
                int id;
                cout << "  Enter Item ID to search: ";
                if (!(cin >> id)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "  [ERROR] Invalid ID.\n"; continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                searchByID(id);

            } else if (choice == 3) {
                
                string keyword;
                cout << "  Enter name keyword: ";
                getline(cin, keyword);
                searchByName(keyword);

            } else if (choice == 4) {
                
                int    id;
                string newName, newLocation;
                cout << "  Enter Item ID to update : ";
                if (!(cin >> id)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "  [ERROR] Invalid ID.\n"; continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Enter new Item Name     : "; getline(cin, newName);
                cout << "  Enter new Item Location : "; getline(cin, newLocation);
                updateItem(id, newName, newLocation);

            } else if (choice == 5) {
                
                int id;
                cout << "  Enter Item ID to delete: ";
                if (!(cin >> id)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "  [ERROR] Invalid ID.\n"; continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                deleteItem(id);

            } else if (choice == 6) {
                
                displayAll();

            } else if (choice == 0) {
                cout << "  Returning to main menu...\n";

            } else {
                cout << "  [ERROR] Invalid choice. Please select 0-6.\n";
            }
        }
    }

    
    void runTask4() {
        ItemManager manager;
        manager.run();
    }

} 


int main() {
    int choice = 0;

    do {
        cout << endl;
        cout << string(50, '=') << endl;
        cout << "  WAREHOUSE ROBOT NAVIGATION SYSTEM" << endl;
        cout << string(50, '=') << endl;
        cout << "  1. Task 1 - Order Management Module" << endl;
        cout << "  2. Task 4 - Item Search & Management Module" << endl;
        cout << "  0. Exit System" << endl;
        cout << string(50, '=') << endl;
        cout << "  Enter your choice: ";
        cin  >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [ERROR] Invalid input. Please enter 0, 1, or 2.\n";
            continue;
        }

        switch (choice) {
        case 1:
            Task1::runTask1();
            break;
        case 2:
            Task4::runTask4();
            break;
        case 0:
            cout << "\n  Exiting system. Goodbye!\n" << endl;
            break;
        default:
            cout << "  [ERROR] Invalid choice. Please enter 0, 1, or 2.\n";
        }
    } while (choice != 0);

    return 0;
}
