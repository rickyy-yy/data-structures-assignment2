#include "Task1.hpp"

int nextOrderID = 1;
ItemArray itemArray;

WaitingQueue waitingQueue;
ProcessingQueue processingQueue(5);
CompletedQueue completedQueue;

int getFileLength(string filepath){
    ifstream thisFile(filepath);

    if(!thisFile.is_open()){
        cout << HEADER << endl;
        cout << "File can't be open!" << endl;
        cout << HEADER << endl;
        return -1;
    }

    string line;
    int count = 0;
    getline(thisFile, line);
    while(getline(thisFile, line)){
        count++;
    }

    return count;
}

void loadItems(){
    ifstream itemFile(ITEMS_FILE);

    if(!itemFile.is_open()){
        cout << HEADER << endl;
        cout << "Item file can't be open!" << endl;
        cout << HEADER << endl;
        return;
    }

    string line;
     if (itemFile.peek() == 0xEF) {
        char bom[3];
        itemFile.read(bom, 3);
    }

    getline(itemFile, line);
    while(getline(itemFile, line)){
        string idString;
        string zoneString;
        string shelfNumberString;

        int itemID;
        string itemName;
        char zone;
        int shelfNumber;

        stringstream stringStream(line);

        getline(stringStream, idString, ',');
        getline(stringStream, itemName, ',');
        getline(stringStream, zoneString, ',');
        getline(stringStream, shelfNumberString, ',');

        itemID = stoi(idString);
        zone = zoneString[0];
        shelfNumber = stoi(shelfNumberString);

        Item* temp = new Item();
        temp->itemID = itemID;
        temp->itemName = itemName;
        temp->shelfNumber = shelfNumber;
        temp->zone = zone;

        itemArray.append(*temp);
    }
    itemFile.close();
}

Order* createOrder(){
    int thisItemID, thisRobotID, thisShelfNumber;
    char thisPackingStation, thisZone;

    cout << HEADER << endl;
    cout << "Order Creation" << endl;
    cout << HEADER << endl;

    while(true){
        cout << "Enter Item ID: ";
        cin >> thisItemID;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000000, '\n');
            cout << endl;
            cout << HEADER << endl;
            cout << "! Invalid input. Please enter an integer." << endl;
            cout << HEADER << endl;
        }
        else if(!itemArray.itemExist(thisItemID)){
            cin.clear();
            cin.ignore(1000000, '\n');
            cout << endl;
            cout << HEADER << endl;
            cout << "! Item does not exist!." << endl;
            cout << HEADER << endl;
        }
        else{
            break;
        }
    }

    while(true){
        cout << "Enter Shelf Number: ";
        cin >> thisShelfNumber;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000000, '\n');
            cout << endl;
            cout << HEADER << endl;
            cout << "! Invalid input. Please enter an integer." << endl;
            cout << HEADER << endl;
        }
        else{
            break;
        }
    }

    while(true){
        cout << "Enter Packing Station: ";
        cin >> thisPackingStation;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000000, '\n');
            cout << endl;
            cout << HEADER << endl;
            cout << "! Invalid input. Please enter a character." << endl;
            cout << HEADER << endl;
        }
        else{
            thisPackingStation = toupper(thisPackingStation);
            break;
        }
    }

    while(true){
        cout << "Enter Zone: ";
        cin >> thisZone;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000000, '\n');
            cout << endl;
            cout << HEADER << endl;
            cout << "! Invalid input. Please enter a character." << endl;
            cout << HEADER << endl;
        }
        else{
            thisZone = toupper(thisZone);
            break;
        }
    }
    
    // CHANGE LATER
    thisRobotID = -1;
    // CHANGE LATER
    
    Order* order = new Order(nextOrderID, thisItemID, thisRobotID, thisShelfNumber, thisPackingStation, thisZone);
    nextOrderID++;
    return order;
}

void runTask1(){
    int choice = 0;
    
    loadItems();

    cout << HEADER << endl;
    cout << "Task 1: Order Management" << endl;
    cout << HEADER << endl;
    while(choice != 7){
        cout << "Task Selection:" << endl;
        cout << "1. Record a new order" << endl;
        cout << "2. Process an order" << endl;
        cout << "3. Complete an order" << endl;
        cout << "4. Display all pending orders" << endl;
        cout << "5. Display all processing orders" << endl;
        cout << "6. Display all completed orders" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice (1-7): ";
        cin >> choice;

        if(cin.fail()){
            cin.clear();
            cin.ignore(1000000, '\n');
            cout << endl;
            cout << HEADER << endl;
            cout << "! Invalid input. Please enter a number between 1 and 7." << endl;
            cout << HEADER << endl;
            continue;
        }
        else if (choice < 1 || choice > 7){
            cout << endl;
            cout << HEADER << endl;
            cout << "! Invalid input. Please enter a number between 1 and 7." << endl;
            cout << HEADER << endl;
            continue;
        }

        switch(choice){
            case 1: {
                // Create Order + Enqueue
                // Create Order
                Order* order = createOrder();
                // Enqueue
                waitingQueue.enqueue(order);
                break;
            }
            case 2: 
                // Process Order (Enqueue + Dequeue)
                if(waitingQueue.isEmpty()){
                    cout << HEADER << endl;
                    cout << "There are no orders to be processed." << endl;
                    cout << HEADER << endl;
                    break;
                }
                if(processingQueue.isFull()){
                    cout << HEADER << endl;
                    cout << "There are no available robots, please wait." << endl;
                    cout << HEADER << endl;
                    break;
                }
                processingQueue.enqueue(waitingQueue.dequeue());
                break;
            case 3:
                // Complete Order (Enqueue + Dequeue)
                if(processingQueue.isEmpty()){
                    cout << HEADER << endl;
                    cout << "There are no orders to be completed." << endl;
                    cout << HEADER << endl;
                    break;
                }
                completedQueue.enqueue(processingQueue.dequeue());
                break;
            case 4:
                waitingQueue.display();
                break;
            case 5:
                processingQueue.display();
                break;
            case 6:
                completedQueue.display();
                break;
            case 7:
                break;
        }
    }
}