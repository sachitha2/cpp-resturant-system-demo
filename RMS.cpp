#include <iostream>
#include <iomanip>
using namespace std;

// Structure to hold menu items
struct MenuItem {
    int itemNo;        // Item number
    string itemName;   // Item name
    float price;       // Item price
};

// Function prototypes
void loadMenu(MenuItem menu[], int size);
void showMenu(const MenuItem menu[], int size);
float takeOrder(const MenuItem menu[], int size, int selectedItems[], int itemCounts[], int& itemCount);
void printInvoice(float subtotal, const MenuItem menu[], const int selectedItems[], const int itemCounts[], int itemCount);

int main() {
    const int menuSize = 8; // Number of items in the menu
    const int maxOrders = 50; // Maximum number of items a customer can order
    MenuItem menu[menuSize]; // Array to store menu items
    int selectedItems[maxOrders]; // Array to store selected item numbers
    int itemCounts[maxOrders];    // Array to store counts of each selected item
    int itemCount = 0; // Counter for selected items
    float subtotal = 0.0; // Total amount before tax

    // Load menu items
    loadMenu(menu, menuSize);

    // Main program loop
    int choice;
    do {
        cout << "\n******** Welcome to Meal Hut ********\n";
        cout << "1. Show Menu\n";
        cout << "2. Place Order\n";
        cout << "3. Print Invoice\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                showMenu(menu, menuSize);
                break;
            case 2:
                subtotal = takeOrder(menu, menuSize, selectedItems, itemCounts, itemCount);
                break;
            case 3:
                printInvoice(subtotal, menu, selectedItems, itemCounts, itemCount);
                break;
            case 4:
                cout << "Exiting the system. Thank you for visiting Meal Hut!\n";
                break;
            default:
                cout << "Invalid choice! Please select again.\n";
        }
    } while (choice != 4);

    return 0;
}

// Function to load menu items into the array
void loadMenu(MenuItem menu[], int size) {
    menu[0] = {111, "Plain Egg", 1.45};
    menu[1] = {112, "Bacon and Egg", 2.45};
    menu[2] = {113, "Muffin     ", 0.99};
    menu[3] = {114, "French Toast", 1.99};
    menu[4] = {115, "Fruit Basket", 2.49};
    menu[5] = {116, "Cereal     ", 0.69};
    menu[6] = {117, "Coffee     ", 0.50};
    menu[7] = {118, "Tea        ", 0.75};
    cout << "Menu items loaded successfully!\n";
}

// Function to display the menu
void showMenu(const MenuItem menu[], int size) {
    cout << "\n******** Meal Hut Menu ********\n";
    cout << "Item No\tMenu Item\t\tPrice\n";
    for (int i = 0; i < size; i++) {
        cout << menu[i].itemNo << "\t" << menu[i].itemName 
             << "\t\t$" << fixed << setprecision(2) << menu[i].price << endl;
    }
}

// Function to take orders from the user
float takeOrder(const MenuItem menu[], int size, int selectedItems[], int itemCounts[], int& itemCount) {
    int itemNo, quantity;
    float subtotal = 0.0;

    cout << "\nEnter the item numbers and quantities you want to order (Enter -1 to finish):\n";
    while (true) {
        cout << "Enter Item Number: ";
        cin >> itemNo;
        if (itemNo == -1) break; // Stop when user enters -1

        bool validItem = false;
        for (int i = 0; i < size; i++) {
            if (menu[i].itemNo == itemNo) {
                cout << "Enter Quantity: ";
                cin >> quantity;

                subtotal += menu[i].price * quantity;
                selectedItems[itemCount] = itemNo;  // Add item number to the selected list
                itemCounts[itemCount] = quantity;  // Add quantity for the selected item
                itemCount++;

                cout << "Added: " << menu[i].itemName << " x" << quantity 
                     << " - $" << fixed << setprecision(2) << menu[i].price * quantity << endl;
                validItem = true;
                break;
            }
        }

        if (!validItem) {
            cout << "Invalid item number. Please try again.\n";
        }
    }

    return subtotal;
}

// Function to print the invoice
void printInvoice(float subtotal, const MenuItem menu[], const int selectedItems[], const int itemCounts[], int itemCount) {
    if (subtotal == 0.0) {
        cout << "No items ordered. Please place an order first!\n";
        return;
    }

    float tax = subtotal * 0.05; // 5% tax
    float total = subtotal + tax;

    cout << "\n******** Your Invoice ********\n";
    cout << "Item No\tMenu Item\t\tQuantity\tPrice\n";

    for (int i = 0; i < itemCount; i++) {
        for (int j = 0; j < 8; j++) {
            if (menu[j].itemNo == selectedItems[i]) {
                cout << menu[j].itemNo << "\t" << menu[j].itemName 
                     << "\t\t" << itemCounts[i] 
                     << "\t\t$" << fixed << setprecision(2) << menu[j].price * itemCounts[i] << endl;
                break;
            }
        }
    }

    cout << "\nSubtotal: $" << fixed << setprecision(2) << subtotal << endl;
    cout << "Tax (5%): $" << fixed << setprecision(2) << tax << endl;
    cout << "Total: $" << fixed << setprecision(2) << total << endl;
    cout << "*******************************\n";
}
