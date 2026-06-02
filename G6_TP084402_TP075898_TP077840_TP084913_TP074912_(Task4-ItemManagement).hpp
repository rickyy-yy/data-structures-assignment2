#pragma once

#include <string>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;

namespace Task4 {

    
    struct Item {
        int    itemID;    
        string itemName;
        string location;  
    };

    
    struct BSTNode {
        Item     data;
        BSTNode* left;    
        BSTNode* right;   

        BSTNode(Item item) : data(item), left(nullptr), right(nullptr) {}
    };

    
    class ItemManager {
    private:
        BSTNode* root;

        
        BSTNode* insertHelper   (BSTNode* node, Item item);
        BSTNode* searchByID     (BSTNode* node, int id);
        void     searchByName   (BSTNode* node, const string& keyword, bool& found);
        BSTNode* findMin        (BSTNode* node);
        BSTNode* deleteHelper   (BSTNode* node, int id);
        void     inOrderDisplay (BSTNode* node, int& count);
        void     destroyTree    (BSTNode* node);
        bool     isDuplicate    (BSTNode* node, int id);

    public:
        ItemManager();
        ~ItemManager();

        void insertItem  (int id, const string& name, const string& location);
        void searchByID  (int id);
        void searchByName(const string& keyword);
        void updateItem  (int id, const string& newName, const string& newLocation);
        void deleteItem  (int id);
        void displayAll  ();
        void showMenu    ();
        void run         ();
    };

    void runTask4();   

} 
