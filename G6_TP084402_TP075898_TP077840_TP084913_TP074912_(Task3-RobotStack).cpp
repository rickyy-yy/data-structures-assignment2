#include "G6_TP084402_TP075898_TP077840_TP084913_TP074912_(Task3-RobotStack).hpp"
#include <iostream>
#include <string>
using namespace std;

//Node Structure
struct Node {
    string move;
    Node* next;

    Node(string m) : move(m), next(nullptr) {}
};

// Navigation's Log Node
struct LogNode {
    string entry;
    LogNode* next;

    LogNode(string e) : entry(e), next(nullptr) {}
};


// Navigation Node Class
class NavigationLog {
private:
    LogNode* head;
    LogNode* tail;
    int      count;

public:
    NavigationLog() : head(nullptr), tail(nullptr), count(0) {}

    ~NavigationLog() {
        LogNode* curr = head;
        while (curr != nullptr) {
            LogNode* temp = curr->next;
            delete curr;
            curr = temp;
        }
    }

    void addEntry(string entry) {
        LogNode* newNode = new LogNode("[" + to_string(++count) + "] " + entry);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void displayLog() {
        if (head == nullptr) {
            cout << "  (No events logged yet)" << endl;
            return;
        }
        LogNode* curr = head;
        while (curr != nullptr) {
            cout << "  " << curr->entry << endl;
            curr = curr->next;
        }
    }

    bool isEmpty() {
        return head == nullptr;
    }
};


// Stack Class
class Stack {
private:
    Node* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

 // Destructor to free all remaining nodes
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

// To push a move onto the top of the stack
    void push(string move) {
        Node* newNode = new Node(move);
        newNode->next = top;
        top = newNode;
        size++;
    }

// To pop the top move off the stack and return it
    string pop() {
        if (isEmpty()) {
            return "";
        }
        string move = top->move;
        Node* temp = top;
        top = top->next;
        delete temp;
        size--;
        return move;
    }

// To peek at the top move without removing it
    string peek() {
        if (isEmpty()) return "None";
        return top->move;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    int getSize() {
        return size;
    }

// Display forward path
    void displayForwardPath() {
        if (isEmpty()) {
            cout << "  [Stack is empty]" << endl;
            return;
        }
        // Collect into array for forward display
        string moves[500];
        int count = 0;
        Node* curr = top;
        while (curr != nullptr && count < 500) {
            moves[count++] = curr->move;
            curr = curr->next;
        }
        for (int i = count - 1; i >= 0; i--) {
            cout << "  Step " << (count - i) << ": " << moves[i] << endl;
        }
    }

// Display reverse path preview
    void displayReversePath() {
        if (isEmpty()) {
            cout << "  [No moves to reverse]" << endl;
            return;
        }
        Node* curr = top;
        int step = 1;
        while (curr != nullptr) {
            cout << "  Return Step " << step++ << ": "
                 << reverseOf(curr->move) << endl;
            curr = curr->next;
        }
    }

    // Returns the opposite of a given move
    static string reverseOf(string move) {
        if (move == "Forward")   return "Backward";
        if (move == "Backward")  return "Forward";
        if (move == "TurnLeft")  return "TurnRight";
        if (move == "TurnRight") return "TurnLeft";
        return "Unknown";
    }
};

//robot navigation
class RobotNavigation {
private:
    Stack         pathStack;
    NavigationLog navLog;
    string        currentLocation;
    int           totalMoves;
    bool          taskComplete;
    bool          returnedToStart;

// Returns the opposite move for reverse navigation
    string reverseMove(string move) {
        return Stack::reverseOf(move);
    }

// Record move into stack and log
    void recordMove(string move, string description) {
        pathStack.push(move);
        totalMoves++;
        navLog.addEntry(description);
        cout << "  [Move " << totalMoves << "] -> " << description << endl;
    }

public:
    RobotNavigation()
        : currentLocation("Packing Station"),
          totalMoves(0), taskComplete(false), returnedToStart(false) {}


//Command for Movements
    void moveForward() {
        recordMove("Forward", "Moved forward");
    }

    void moveBackward() {
        recordMove("Backward", "Moved backward");
    }

    void turnLeft() {
        recordMove("TurnLeft", "Turned left");
    }

    void turnRight() {
        recordMove("TurnRight", "Turned right");
    }

//Mark Item picked up
    void markTaskComplete() {
        taskComplete = true;
        navLog.addEntry("Item picked up - task marked complete");
        cout << "  [Task Complete] Item picked up." << endl;
    }

// Obstacles Handler
    void handleObstacle() {
        if (pathStack.isEmpty()) {
            cout << "  [Obstacle] No moves to backtrack!" << endl;
            return;
        }
        string lastMove = pathStack.pop();
        totalMoves--;
        string backtrack = reverseMove(lastMove);
        navLog.addEntry("[OBSTACLE] Backtracked: " + backtrack);
        cout << "  [Obstacle detected!] Backtracking: " << backtrack << endl;
    }

// Return to Start
    void returnToStart() {
        cout << "\n________________________________________" << endl;
        cout << "  Robot returning to start" << endl;
        cout << "________________________________________" << endl;

        if (pathStack.isEmpty()) {
            cout << "  Already at start position!" << endl;
            returnedToStart = true;
            return;
        }

        int step = 1;
        while (!pathStack.isEmpty()) {
            string lastMove = pathStack.pop();
            string reverse  = reverseMove(lastMove);
            navLog.addEntry("Return step " + to_string(step) + ": " + reverse);
            cout << "  [Return step " << step++ << "] Executing: "
                 << reverse << " (reversing: " << lastMove << ")" << endl;
        }

        returnedToStart = true;
        totalMoves = 0;
        navLog.addEntry("Robot returned to " + currentLocation);
        cout << "Robot has returned to: " << currentLocation << endl;
    }

// Display the Utilities
    void showForwardPath() {
        cout << "\n  Forward path (first move to latest):" << endl;
        pathStack.displayForwardPath();
    }

    void showReversePath() {
        cout << "\n  Reverse path preview (return journey):" << endl;
        pathStack.displayReversePath();
    }

    void showFullLog() {
        cout << "\n  Complete Navigation Log:" << endl;
        navLog.displayLog();
    }

    void showStatus() {
        cout << "\n    Robot Status    " << endl;
        cout << "  Location   : " << currentLocation << endl;
        cout << "  Moves taken: " << pathStack.getSize() << endl;
        cout << "  Task done  : " << (taskComplete    ? "Yes" : "No") << endl;
        cout << "  Returned   : " << (returnedToStart ? "Yes" : "No") << endl;
        cout << "  Next return: " << pathStack.peek() << " (will reverse first)" << endl;
    }

// Getters integration
    bool isTaskComplete() { return taskComplete;    }
    bool hasReturned()    { return returnedToStart; }
};


//Display Menu
void displayMenu() {
    cout << "________________________________________" << endl;
    cout << "  TASK 3: ROBOT NAVIGATION MODULE"       << endl;
    cout << "________________________________________" << endl;
    cout << "  Navigation Menu" << endl;
    cout << "  1.  Move Forward"                      << endl;
    cout << "  2.  Move Backward"                     << endl;
    cout << "  3.  Turn Left"                         << endl;
    cout << "  4.  Turn Right"                        << endl;
    cout << "  5.  Handle Obstacle"                   << endl;
    cout << "  6.  Mark Item as Picked Up"            << endl;
    cout << "  7.  Show Forward Path"                 << endl;
    cout << "  8.  Show Reverse Path Preview"         << endl;
    cout << "  9.  Return to Start"                   << endl;
    cout << "  10. Show Full Navigation Log"          << endl;
    cout << "  11. Show Robot Status"                 << endl;
    cout << "  0.  Exit Navigation Module"            << endl;
    cout << "________________________________________" << endl;
    cout << "  Choice: ";
}

//to combine with group
void navigateRobot() {
    RobotNavigation robot;

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:  robot.moveForward();      break;
            case 2:  robot.moveBackward();     break;
            case 3:  robot.turnLeft();         break;
            case 4:  robot.turnRight();        break;
            case 5:  robot.handleObstacle();   break;
            case 6:  robot.markTaskComplete(); break;
            case 7:  robot.showForwardPath();  break;
            case 8:  robot.showReversePath();  break;
            case 9:  robot.returnToStart();    break;
            case 10: robot.showFullLog();      break;
            case 11: robot.showStatus();       break;
            case 0:
                cout << "\n  Exiting navigation module." << endl;
                robot.showFullLog();
                cout << "  Returning control to main system..." << endl;
                break;
            default:
                cout << "  Invalid choice. Try again." << endl;
        }
    } while (choice != 0);
}

int main() {
    navigateRobot();

    return 0;
}
