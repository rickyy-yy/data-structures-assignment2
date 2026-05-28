#include "Task1.hpp"

int nextOrderID = 1;
ItemArray itemArray;

WaitingQueue waitingQueue;
ProcessingQueue processingQueue;
CompletedQueue completedQueue;

int getFileLength(string filepath){
    ifstream thisFile(filepath);

    if(!thisFile.is_open()){
        cout << HEADER << endl;
        cout << "File can't be open!" << endl;
        cout << HEADER << endl;
        return 0;
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

        Item item;
        item.itemID = itemID;
        item.itemName = itemName;
        item.shelfNumber = shelfNumber;
        item.zone = zone;

        itemArray.append(item);
    }
    itemFile.close();
    itemArray.recursion(itemArray.array, 0, itemArray.size - 1);
}

void loadWaitingQueue(){
    ifstream waitingQueueFile(WAITING_QUEUE_FILE);

    if(!waitingQueueFile.is_open()){
        cout << HEADER << endl;
        cout << "Waiting queue file can't be open!" << endl;
        cout << HEADER << endl;
        return;
    }

    string line;
    getline(waitingQueueFile, line);
    while(getline(waitingQueueFile, line)){
        int orderID;
        int itemID;
        int shelfNumber;
        char zone;
        char packingStation;

        string orderIDString;
        string itemIDString;
        string shelfNumberString;
        string zoneString;
        string packingStationString;
        string robotIDString;

        stringstream stringStream(line);

        getline(stringStream, orderIDString, ',');
        getline(stringStream, itemIDString, ',');
        getline(stringStream, shelfNumberString, ',');
        getline(stringStream, zoneString, ',');
        getline(stringStream, packingStationString, ',');
        getline(stringStream, robotIDString, ',');

        orderID = stoi(orderIDString);
        itemID = stoi(itemIDString);
        zone = zoneString[0];
        shelfNumber = stoi(shelfNumberString);
        packingStation = packingStationString[0];

        Order* order = new Order();
        order->orderID = orderID;
        order->itemID = itemID;
        order->shelfNumber = shelfNumber;
        order->zone = zone;
        order->packingStation = packingStation;
        order->robotID = -1;

        waitingQueue.loading = true;
        waitingQueue.enqueue(order);
        if(orderID >= nextOrderID){
            nextOrderID = orderID + 1;
        }
    }
    waitingQueue.loading = false;
    waitingQueueFile.close();
}

void loadCompletedQueue(){
    ifstream completedQueueFile(COMPLETED_QUEUE_FILE);

    if(!completedQueueFile.is_open()){
        cout << HEADER << endl;
        cout << "Completed queue file can't be open!" << endl;
        cout << HEADER << endl;
        return;
    }

    string line;
    getline(completedQueueFile, line);
    while(getline(completedQueueFile, line)){
        int orderID;
        int itemID;
        int shelfNumber;
        char zone;
        char packingStation;
        int robotID;

        string orderIDString;
        string itemIDString;
        string shelfNumberString;
        string zoneString;
        string packingStationString;
        string robotIDString;

        stringstream stringStream(line);

        getline(stringStream, orderIDString, ',');
        getline(stringStream, itemIDString, ',');
        getline(stringStream, shelfNumberString, ',');
        getline(stringStream, zoneString, ',');
        getline(stringStream, packingStationString, ',');
        getline(stringStream, robotIDString, ',');

        orderID = stoi(orderIDString);
        itemID = stoi(itemIDString);
        zone = zoneString[0];
        shelfNumber = stoi(shelfNumberString);
        packingStation = packingStationString[0];
        robotID = stoi(robotIDString);

        Order* order = new Order();
        order->orderID = orderID;
        order->itemID = itemID;
        order->shelfNumber = shelfNumber;
        order->zone = zone;
        order->packingStation = packingStation;
        order->robotID = robotID;

        completedQueue.loading = true;
        completedQueue.enqueue(order);
        if(orderID >= nextOrderID){
            nextOrderID = orderID + 1;
        }
    }
    completedQueue.loading = false;
    completedQueueFile.close();
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
            cout << endl;
        }
        else{
            break;
        }
    }

    Item thisItem = itemArray.search(thisItemID);
    thisShelfNumber = thisItem.shelfNumber;
    thisZone = thisItem.zone;

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

    // CHANGE LATER
    thisRobotID = -1;
    // CHANGE LATER
    
    Order* order = new Order(nextOrderID, thisItemID, thisRobotID, thisShelfNumber, thisPackingStation, thisZone);
    nextOrderID++;
    return order;
}

void runTask1(){
    int choice = 0;
    
    if(processingQueue.capacity == 0){
        int robot_count = getFileLength(ROBOTS_FILE);
        processingQueue.resize(robot_count);
    }
    
    int item_count = getFileLength(ITEMS_FILE);
    itemArray.init(item_count);

    waitingQueue.clear();
    completedQueue.clear();

    loadWaitingQueue();
    loadCompletedQueue();

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
            case 3: {
                // Complete Order (Enqueue + Dequeue)
                if(processingQueue.isEmpty()){
                    cout << HEADER << endl;
                    cout << "There are no orders to be completed." << endl;
                    cout << HEADER << endl;
                    break;
                }
                Order* doneOrder = processingQueue.dequeue();
                completedQueue.enqueue(doneOrder);
                break;
            }
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