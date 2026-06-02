#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task1-OrderManagement).hpp"
#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task2-RobotManagement).hpp"

// ItemArray Struct

ItemArray::ItemArray(){
    capacity = 0;
    size = 0;
    pointer = -1;
    array = nullptr;
}

ItemArray::ItemArray(int maxSize){
    pointer = -1;
    size = 0;
    capacity = maxSize;
    array = new Item[maxSize];
}

ItemArray::~ItemArray(){
    delete[] array;
}

void ItemArray::init(int maxSize){
    delete[] array;
    capacity = maxSize;
    size = 0;
    pointer = -1;
    array = new Item[maxSize];
}

bool ItemArray::isEmpty(){
    return size == 0;
}

void ItemArray::append(Item item){
    if(size >= capacity){
        cout << HEADER << endl;
        cout << "Array is full!" << endl;
        cout << HEADER << endl;
        return;
    }
    pointer++;
    array[pointer] = item;
    size++;
}

void ItemArray::display(){
    if(isEmpty()){
        cout << HEADER << endl;
        cout << "Array is empty!" << endl;
        cout << HEADER << endl;
        return;
    }
    int tempPointer = 0;
    
    cout << left
            << setw(10) << "Item ID"
            << "| " 
            << setw(10) << "Item Name"
            << "| " 
            << setw(10) << "Shelf Number"
            << "| " 
            << setw(10) << "Zone"
            << endl;
            
    while(tempPointer <= pointer){
        cout << left
                << setw(10) << array[tempPointer].itemID                 
                << "| "
                << setw(10) << array[tempPointer].itemName
                << "| "
                << setw(10) << array[tempPointer].shelfNumber
                << "| "
                << setw(10) << array[tempPointer].zone
                << endl;
        tempPointer++;
    }
}

void ItemArray::recursion(Item array[], int lowerBound, int upperBound){
    if(lowerBound < upperBound){
        int pivot = partition(array, lowerBound, upperBound);
        recursion(array, lowerBound, pivot - 1);
        recursion(array, pivot + 1, upperBound);
    }
    else{
        return;
    }
}

int ItemArray::partition(Item array[], int lowerBound, int upperBound){
    int pivot = array[upperBound].itemID;
    int i = lowerBound;

    for(int j = lowerBound; j < upperBound; j++){
        if(array[j].itemID <= pivot){
            swap(array[i], array[j]);
            i++;
        }
    }
    swap(array[i], array[upperBound]);
    return i;
}

Item ItemArray::search(int queryID){
    if(queryID < 1){
        return Item();
    }
    int lowerBound = 0;
    int upperBound = size - 1;

    while(lowerBound <= upperBound){
        int mid = (lowerBound + upperBound) / 2;

        if(array[mid].itemID == queryID){
            return array[mid];
        }
        else if(array[mid].itemID < queryID){
            lowerBound = mid + 1;
        }
        else{
            upperBound = mid - 1;
        }
    }
    return Item();
}

bool ItemArray::itemExist(int queryID){
    int lowerBound = 0;
    int upperBound = size - 1;

    while(lowerBound <= upperBound){
        int mid = (lowerBound + upperBound) / 2;

        if(array[mid].itemID == queryID){
            return true;
        }
        else if(array[mid].itemID < queryID){
            lowerBound = mid + 1;
        }
        else{
            upperBound = mid - 1;
        }
    }
    return false;
}

// Order Struct

Order::Order(){
        orderID = -1;
        itemID = -1;
        robotID = -1;
        shelfNumber = -1;
        packingStation = 'A';
        zone = 'A';
        status = "Pending";
        next = nullptr;
}

Order::Order(int thisOrderID, int thisItemID, int thisRobotID, int thisShelfNumber, char thisPackingStation, char thisZone){
        orderID = thisOrderID;
        itemID = thisItemID;
        robotID = thisRobotID;
        shelfNumber = thisShelfNumber;
        packingStation = thisPackingStation;
        zone = thisZone;
        status = "Pending";
        next = nullptr;
}

// WaitingQueue Struct

WaitingQueue::WaitingQueue(){
        loading = false;
        front = nullptr;
        rear = nullptr;
        size = 0;
    }

WaitingQueue::~WaitingQueue(){
        Order* current = front;
        while(current != nullptr){
            Order* temp = current->next;
            delete current;
            current = temp;
        }
    }

bool WaitingQueue::isEmpty(){
        if(size == 0){
            return true;
        }
        return false;
    }

void WaitingQueue::enqueue(Order* order){
        order->next = nullptr;

        if(rear == nullptr){
            front = order;
            rear = order;
            size++;

            if(loading){
                return;
            }

            cout << endl << HEADER << endl;
            cout << "Order #" << order->orderID << " is now waiting to be processed." << endl;
            cout << HEADER << endl;
            
            updateFile();

            return;
        }
        rear->next = order;
        rear = order;
        size++;

        if(loading){
            return;
        }

        cout << endl << HEADER << endl;
        cout << "Order #" << order->orderID << " is now waiting to be processed." << endl;
        cout << HEADER << endl;

        updateFile();

        return;
    }

Order* WaitingQueue::dequeue(){
        if(isEmpty()){
            cout << HEADER << endl;
            cout << "Queue empty!" << endl;
            cout << HEADER << endl;
            return nullptr;
        }
        Order* temp = front;
        front = front->next;
        size--;
        updateFile();

        if(front == nullptr){
            rear = nullptr;  
        }
        return temp;
    }

void WaitingQueue::display(){
        if(isEmpty()){
            cout << HEADER << endl;
            cout << "Queue empty!" << endl;
            cout << HEADER << endl;
            return;
        }
        Order* current = front;

        cout << HEADER << endl;
        cout << left
             << setw(10) << "Order ID"
             << "| " 
             << setw(10) << "Item ID"
             << "| "
             << setw(10) << "Shelf"
             << "| "
             << setw(12) << "Station"
             << "| "
             << setw(8)  << "Zone"
             << "| "
             << setw(10) << "Robot ID"
             << "| "
             << setw(15) << "Status"
             << endl;
        cout << HEADER << endl;
        while(current != nullptr){
            cout << left
                 << setw(10) << current->orderID
                 << "| "
                 << setw(10) << current->itemID
                 << "| "
                 << setw(10) << current->shelfNumber
                 << "| "
                 << setw(12) << current->packingStation
                 << "| "
                 << setw(8)  << current->zone
                 << "| "
                 << setw(10) << (current->robotID == -1 ? "None" : to_string(current->robotID))
                 << "| "
                 << setw(15) << current->status
                 << endl;
            current = current->next;
        }
    }

void WaitingQueue::updateFile(){
        ofstream waitingQueueFile(WAITING_QUEUE_FILE);

        if(!waitingQueueFile.is_open()){
            cout << HEADER << endl;
            cout << "Waiting queue file can't be open!" << endl;
            cout << HEADER << endl;
            return;
        }

        if(!isEmpty()){
            waitingQueueFile << "orderID,itemID,shelfNumber,zone,packingStation" << "\n";
            Order* current = front;
            while(current != nullptr){
                waitingQueueFile << current->orderID << "," << current->itemID << "," << current->shelfNumber << "," << current->zone << "," << current->packingStation << "\n";
                current = current->next;
            }
        }
        else{
            waitingQueueFile << "orderID,itemID,shelfNumber,zone,packingStation" << "\n";
        }

        waitingQueueFile.close();
    }

void WaitingQueue::clear(){
        Order* current = front;
        while(current != nullptr){
            Order* temp = current->next;
            delete current;
            current = temp;
        }
        front = nullptr;
        rear = nullptr;
        size = 0;
    }

// ProcessingQueue Struct

ProcessingQueue::ProcessingQueue(){
        capacity = 0;
        queue = nullptr;
        front = 0;
        rear = -1;
        size = 0;
        loading = false;
}

ProcessingQueue::~ProcessingQueue(){
    for(int i = 0; i < size; i++){
        int index  = (front + i) % capacity;
        delete queue[index];
    }
    delete[] queue;
}

void ProcessingQueue::init(int maxSize){
    for(int i = 0; i < size; i++){
        int index  = (front + i) % capacity;
        delete queue[index];
    }
    delete[] queue;

    capacity = maxSize;
    queue = new Order*[capacity];
    front = 0;
    rear = -1;
    size = 0;
}

void ProcessingQueue::resize(int newCapacity){
    if(capacity == 0){
        init(newCapacity);
        return;
    }
    if(newCapacity == capacity){
        return;
    }
    if(newCapacity < size){
        newCapacity = size;
    }

    Order** newQueue = new Order*[newCapacity];

    for(int i = 0; i < size; i++){
        int index = (front + i) % capacity;
        newQueue[i] = queue[index];
    }

    delete[] queue;

    queue = newQueue;
    capacity = newCapacity;
    front = 0;
    if(size == 0){
        rear = -1;
    }
    else{
        rear = size - 1;
    }
    
}

void ProcessingQueue::enqueue(Order* order, int robotID){
    if(size >= capacity){
        cout << "Queue full!" << endl;
        return;
    }
    rear = (rear + 1) % capacity;
    queue[rear] = order;
    size++;
    order->status = "Processing";

    if(order->robotID <= 0){
        order->robotID = robotID;
    }

    if(loading){
        return;
    }

    updateFile();

    cout << HEADER << endl;
    cout << "Order #" << order->orderID << " is now assigned to Robot #" << order->robotID << endl;
    cout << HEADER << endl;

    return;
}

Order* ProcessingQueue::dequeue(){
    if(isEmpty()){
        cout << HEADER << endl;
        cout << "Queue empty!" << endl;
        cout << HEADER << endl;
        return nullptr;
    }
    
    Order* o = queue[front];
    front = (front + 1) % capacity;
    size--;

    updateFile();

    return o;
}

void ProcessingQueue::display(){
    if(isEmpty()){
        cout << HEADER << endl;
        cout << "Queue empty!" << endl;
        cout << HEADER << endl;
        return;
    }

    cout << HEADER << endl;
    cout << left
            << setw(10) << "Order ID"
            << "| " 
            << setw(10) << "Item ID"
            << "| "
            << setw(10) << "Shelf"
            << "| "
            << setw(12) << "Station"
            << "| "
            << setw(8)  << "Zone"
            << "| "
            << setw(10) << "Robot ID"
            << "| "
            << setw(15) << "Status"
            << endl;
    cout << HEADER << endl;
    
    for(int i = 0; i < size; i++){
        int index = (front + i) % capacity;
        cout << left
                << setw(10) << queue[index]->orderID
                << "| "
                << setw(10) << queue[index]->itemID
                << "| "
                << setw(10) << queue[index]->shelfNumber
                << "| "
                << setw(12) << queue[index]->packingStation
                << "| "
                << setw(8)  << queue[index]->zone
                << "| "
                << setw(10) << (queue[index]->robotID == -1 ? "None" : to_string(queue[index]->robotID))
                << "| "
                << setw(15) << queue[index]->status
                << endl;
    }
}

void ProcessingQueue::updateFile(){
    ofstream processingQueueFile(PROCESSING_QUEUE_FILE);

    if(!processingQueueFile.is_open()){
        cout << HEADER << endl;
        cout << "Processing queue file can't be open!" << endl;
        cout << HEADER << endl;
        return;
    }

    if(!isEmpty()){
        processingQueueFile << "orderID,itemID,shelfNumber,zone,packingStation,robotID" << "\n";
        for(int i = 0; i < size; i++){
            int index = (front + i) % capacity;
            processingQueueFile << queue[index]->orderID << "," << queue[index]->itemID << "," << queue[index]->shelfNumber << "," << queue[index]->zone << "," << queue[index]->packingStation << "," << queue[index]->robotID << "\n";
        }
    }
    else{
        processingQueueFile << "orderID,itemID,shelfNumber,zone,packingStation,robotID" << "\n";
    }
    processingQueueFile.close();
}

bool ProcessingQueue::isEmpty(){
    return size == 0;
}

bool ProcessingQueue::isFull(){
    return size == capacity;
}

void ProcessingQueue::clear(){
    for(int i = 0; i< size; i++){
        int index = (front + i) % capacity;
        delete queue[index];
    }

    front = 0;
    rear = -1;
    size = 0;
}

// CompletedQueue Struct

CompletedQueue::CompletedQueue(){
    loading = false;
    front = nullptr;
    rear = nullptr;
    size = 0;
}

CompletedQueue::~CompletedQueue(){
    Order* current = front;
    while(current != nullptr){
        Order* temp = current->next;
        delete current;
        current = temp;
    }
}

bool CompletedQueue::isEmpty(){
    if(size == 0){
        return true;
    }
    return false;
}

void CompletedQueue::enqueue(Order* order){
    order->next = nullptr;
    
    if(rear == nullptr){
        front = order;
        rear = order;
        size++;

        order->status = "Completed";

        if(loading){
            return;
        }

        cout << HEADER << endl;
        cout << "Order #" << order->orderID << " has been processed." << endl;
        cout << HEADER << endl;

        updateFile();

        return;
    }
    rear->next = order;
    rear = order;
    size++;

    order->status = "Completed";

    if(loading){
            return;
        }

    cout << HEADER << endl;
    cout << "Order #" << order->orderID << " has been processed." << endl;
    cout << HEADER << endl;
    
    updateFile();
    
    return;
}

void CompletedQueue::display(){
    if(isEmpty()){
        cout << HEADER << endl;
        cout << "Queue empty!" << endl;
        cout << HEADER << endl;
        return;
    }
    Order* current = front;

    cout << HEADER << endl;
    cout << left
            << setw(10) << "Order ID"
            << "| " 
            << setw(10) << "Item ID"
            << "| "
            << setw(10) << "Shelf"
            << "| "
            << setw(12) << "Station"
            << "| "
            << setw(8)  << "Zone"
            << "| "
            << setw(10) << "Robot ID"
            << "| "
            << setw(15) << "Status"
            << endl;
    cout << HEADER << endl;
    while(current != nullptr){
        cout << left
                << setw(10) << current->orderID
                << "| "
                << setw(10) << current->itemID
                << "| "
                << setw(10) << current->shelfNumber
                << "| "
                << setw(12) << current->packingStation
                << "| "
                << setw(8)  << current->zone
                << "| "
                << setw(10) << (current->robotID == -1 ? "None" : to_string(current->robotID))
                << "| "
                << setw(15) << current->status
                << endl;
        current = current->next;
    }
}

void CompletedQueue::updateFile(){
    ofstream completedQueueFile(COMPLETED_QUEUE_FILE);

    if(!completedQueueFile.is_open()){
        cout << HEADER << endl;
        cout << "Completed queue file can't be open!" << endl;
        cout << HEADER << endl;
        return;
    }

    if(!isEmpty()){
        completedQueueFile << "orderID,itemID,shelfNumber,zone,packingStation,robotID" << "\n";
        Order* current = front;
        while(current != nullptr){
            completedQueueFile << current->orderID << "," << current->itemID << "," << current->shelfNumber << "," << current->zone << "," << current->packingStation << "," << current->robotID << "\n";
            current = current->next;
        }
    }
    else{
        completedQueueFile << "orderID,itemID,shelfNumber,zone,packingStation,robotID" << "\n";
    }

    completedQueueFile.close();
}

void CompletedQueue::clear(){
    Order* current = front;
    while(current != nullptr){
        Order* temp = current->next;
        delete current;
        current = temp;
    }
    front = nullptr;
    rear = nullptr;
    size = 0;
}

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