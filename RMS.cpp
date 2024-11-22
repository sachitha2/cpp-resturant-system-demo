#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;

// Structure for menu items
struct MenuItem {
    int itemNo;        
    string itemName;   
    float price;       
};

// Function prototypes
void loadMenuFromFile(MenuItem menu[], int& size);
void showMenu(const MenuItem menu[], int size);
float takeOrder(const MenuItem menu[], int size, int selectedItems[], int itemCounts[], int& itemCount);
void printInvoice(float subtotal, const MenuItem menu[], const int selectedItems[], const int itemCounts[], int itemCount);


// Generate a timestamp-based filename
std::string generateTimestampFilename();

// Save the order to a unique file -> time stamp-base file
void saveOrderToFile(float subtotal, const MenuItem menu[], const int selectedItems[], const int itemCounts[], int itemCount);



int main() {
    const int maxMenuSize = 50; // Maximum number of items in the menu
    const int maxOrders = 20;  // Maximum number of items a customer can order
    MenuItem menu[maxMenuSize]; // Array to store menu items
    int selectedItems[maxOrders]; // Array to store selected item numbers
    int itemCounts[maxOrders];    // Array to store counts of each selected item
    int menuSize = 0; // Actual number of items in the menu
    int itemCount = 0; // Counter for selected items
    float subtotal = 0.0; // Total amount before tax

    // Load menu items from file -> menu.txt
    loadMenuFromFile(menu, menuSize);

    // Main loop
    int choice;
    do {
        cout << "\n******** Welcome to Meal Hut ********\n";
        cout << "1. Show Menu\n";
        cout << "2. Place Order\n";
        cout << "3. Print Invoice\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Validate user input
        // Validate user input
        if (cin.fail()) {
            cin.clear();                  // Reset the input stream
            cin.ignore(1000, '\n');       // Discard invalid input
            cout << "Invalid input! Please enter a number between 1 and 4.\n";
            continue;                     // Restart the loop
        }

         // Check if the input is out of range
        if (choice < 1 || choice > 4) {
            cout << "Invalid choice! Please enter a number between 1 and 4.\n";
            continue;                     // Restart the loop
        }

        switch (choice) {
            case 1:
                showMenu(menu, menuSize);
                break;
            case 2:
                subtotal = takeOrder(menu, menuSize, selectedItems, itemCounts, itemCount);
                break;
            case 3:
                printInvoice(subtotal, menu, selectedItems, itemCounts, itemCount);
                saveOrderToFile(subtotal, menu, selectedItems, itemCounts, itemCount);
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

// Function to load menu items from a file
void loadMenuFromFile(MenuItem menu[], int& size) {
    ifstream file("menu.txt");
    if (!file) {
        cerr << "Error: Could not open menu file.\n";
        exit(1);
    }
    size = 0;
    while (file >> menu[size].itemNo) {
        file.ignore(); // Ignore the space after the item number
        getline(file, menu[size].itemName, ','); // Read the item name up to the comma
        file >> menu[size].price;
        size++;
    }
    file.close();
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

float takeOrder(const MenuItem menu[], int size, int selectedItems[], int itemCounts[], int& itemCount) {
    // Remove old order data by resetting itemCount
    itemCount = 0; // Reset itemCount to start fresh for the new order
    
    int itemNo, quantity;
    float subtotal = 0.0;

    cout << "\nEnter the item numbers and quantities you want to order (Enter -1 to finish):\n";
    while (true) {
        cout << "Enter Item Number: ";
        cin >> itemNo;

        // Validate user input for item number
        if (cin.fail()) {
            cin.clear();                  // Reset the input stream
            cin.ignore(1000, '\n');       // Discard invalid input
            cout << "Invalid input! Please enter a valid item number.\n";
            continue;
        }

        if (itemNo == -1) break; // Stop when user enters -1

        bool validItem = false;
        for (int i = 0; i < size; i++) {
            if (menu[i].itemNo == itemNo) {
                validItem = true;

                // Repeatedly prompt for a valid quantity
                while (true) {
                    cout << "Enter Quantity: ";
                    cin >> quantity;

                    // Validate user input for quantity
                    if (cin.fail() || quantity <= 0) {
                        cin.clear();                  // Reset the input stream
                        cin.ignore(1000, '\n');       // Discard invalid input
                        cout << "Invalid quantity! Please enter a positive number.\n";
                        continue; // Prompt for quantity again
                    }
                    break; // Exit the loop if quantity is valid
                }

                // Add the item to the order
                subtotal += menu[i].price * quantity;
                selectedItems[itemCount] = itemNo;  // Add item number to the selected list
                itemCounts[itemCount] = quantity;  // Add quantity for the selected item
                itemCount++;

                cout << "Added: " << menu[i].itemName << " x" << quantity 
                     << " - $" << fixed << setprecision(2) << menu[i].price * quantity << endl;
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
        for (int j = 0; j < 50; j++) {
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


// Helper function to generate a timestamp-based filename
string generateTimestampFilename() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    stringstream ss;
    ss << "order_" << 1900 + ltm->tm_year << "_" 
       << 1 + ltm->tm_mon << "_" 
       << ltm->tm_mday << "_"
       << ltm->tm_hour << "_" 
       << ltm->tm_min << "_" 
       << ltm->tm_sec << ".txt";

    return ss.str();
}

// Function to save the order to a unique file
void saveOrderToFile(float subtotal, const MenuItem menu[], const int selectedItems[], const int itemCounts[], int itemCount) {
    string fileName = generateTimestampFilename();
    ofstream file(fileName);
    if (!file) {
        cerr << "Error: Could not save the order to file.\n";
        return;
    }

    float tax = subtotal * 0.05; // 5% tax
    float total = subtotal + tax;
    int totalItemCount = 0; // To track total items ordered

    file << "******** Order Backup ********\n";
    file << "Item No\tMenu Item\t\tQuantity\tPrice\n";

    for (int i = 0; i < itemCount; i++) {
        for (int j = 0; j < 50; j++) {
            if (menu[j].itemNo == selectedItems[i]) {
                file << menu[j].itemNo << "\t" << menu[j].itemName 
                     << "\t\t" << itemCounts[i] 
                     << "\t\t$" << fixed << setprecision(2) << menu[j].price * itemCounts[i] << endl;
                totalItemCount += itemCounts[i]; // Add item quantity to total count
                break;
            }
        }
    }

    file << "\nSubtotal (Total Sale Price): $" << fixed << setprecision(2) << subtotal << endl;
    file << "Tax (5%): $" << fixed << setprecision(2) << tax << endl;
    file << "Final Total: $" << fixed << setprecision(2) << total << endl;
    file << "Total Items Ordered: " << totalItemCount << endl; // Total items count
    file.close();

    cout << "Order saved to " << fileName << " successfully!\n";

}