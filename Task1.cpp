#include "Task1.hpp"
#include "Task2.hpp"

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

int getAvailableRobots(){
    ifstream thisFile(ROBOTS_FILE);

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
        string status;
        string robotIDString;

        stringstream stringStream(line);

        getline(stringStream, robotIDString, ',');
        getline(stringStream, status, ',');

        if(status == "Available"){
            count++;
        }
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

        stringstream stringStream(line);

        getline(stringStream, orderIDString, ',');
        getline(stringStream, itemIDString, ',');
        getline(stringStream, shelfNumberString, ',');
        getline(stringStream, zoneString, ',');
        getline(stringStream, packingStationString, ',');

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

void loadProcessingQueue(){
    ifstream processingQueueFile(PROCESSING_QUEUE_FILE);

    if(!processingQueueFile.is_open()){
        cout << HEADER << endl;
        cout << "Processing queue file can't be open!" << endl;
        cout << HEADER << endl;
        return;
    }

    string line;
    getline(processingQueueFile, line);
    while(getline(processingQueueFile, line)){
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

        processingQueue.loading = true;
        processingQueue.enqueue(order, robotID);
        if(orderID >= nextOrderID){
            nextOrderID = orderID + 1;
        }
    }
    processingQueue.loading = false;
    processingQueueFile.close();
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

    thisRobotID = -1;
    
    Order* order = new Order(nextOrderID, thisItemID, thisRobotID, thisShelfNumber, thisPackingStation, thisZone);
    nextOrderID++;
    return order;
}

void runTask1(){
    int choice = 0;
    int robot_count = getFileLength(ROBOTS_FILE);

    if(processingQueue.capacity != robot_count){
        processingQueue.resize(robot_count);
    }
    
    int item_count = getFileLength(ITEMS_FILE);
    itemArray.init(item_count);

    waitingQueue.clear();
    processingQueue.clear();
    completedQueue.clear();
    robotQueue.clear();

    loadWaitingQueue();
    loadProcessingQueue();
    loadCompletedQueue();
    loadItems();
    robotQueue.loadFromCSV(ROBOTS_FILE);

    for(int i = 0; i < processingQueue.size; i++){
        int processingIndex = (processingQueue.front + i) % processingQueue.capacity;
        Order* temp = processingQueue.queue[processingIndex];
        for(int j = 0; j < robotQueue.count; j++){
            int robotIndex = (robotQueue.front + j) % robotQueue.cap;
            if(robotQueue.slots[robotIndex].robotID == temp->robotID){
                robotQueue.slots[robotIndex].status = "Busy";
                robotQueue.slots[robotIndex].assignedOrderId = temp->orderID;
                break;
            }
        }
    }

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
            case 2: {
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

                // Find an available robot (round-robin)
                int assignedSlot = -1;
                int index;
                for(int attempt = 0; attempt < robotQueue.count; attempt++){
                    index = (robotQueue.front + robotQueue.currentTurn) % robotQueue.cap;
                    
                    //Move turn pointer
                    robotQueue.currentTurn = (robotQueue.currentTurn + 1) % robotQueue.count;
                    
                    //Chk if robot is available
                    if(robotQueue.slots[index].status == "Available"){
                        assignedSlot = index;
                        break;
                    }
                }

                if(assignedSlot == -1){
                    cout << HEADER << endl;
                    cout << "There are no available robots, please wait." << endl;
                    cout << HEADER << endl;
                    break;
                }

                Order* processedOrder = waitingQueue.dequeue();

                // Assign robot to order
                robotQueue.slots[assignedSlot].status = "Busy";
                robotQueue.slots[assignedSlot].assignedOrderId = processedOrder->orderID;
                processedOrder->robotID = robotQueue.slots[assignedSlot].robotID;
                processedOrder->status = "Processing";

                //Save updated robot data
                robotQueue.saveToCSV(ROBOTS_FILE);

                processingQueue.enqueue(processedOrder, robotQueue.slots[assignedSlot].robotID);
                break;
            }
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

                for(int i = 0; i < robotQueue.count; i++){
                    int index = (robotQueue.front + i) % robotQueue.cap;
                    if(robotQueue.slots[index].robotID == doneOrder->robotID){
                        robotQueue.slots[index].status = "Available";
                        robotQueue.slots[index].assignedOrderId = -1;
                        robotQueue.slots[index].totalTaskDone++;
                        break;
                    }
                }
                robotQueue.saveToCSV(ROBOTS_FILE);
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