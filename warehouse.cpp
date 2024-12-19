#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

// Class to represent an inventory item
class Item {
public:
    int itemID;
    string name;
    string category;
    int quantity;
    int reorderLevel;

    Item(int id, string n, string c, int q, int r)
        : itemID(id), name(n), category(c), quantity(q), reorderLevel(r) {}

    void display() const {
        cout << left << setw(10) << itemID
             << setw(20) << name
             << setw(15) << category
             << setw(10) << quantity
             << setw(15) << reorderLevel << endl;
    }

    bool needsRestock() const {
        return quantity < reorderLevel;
    }
};

// Class to manage the inventory system
class InventorySystem {
private:
    vector<Item> items;

public:
    void addItem() {
        int id, quantity, reorderLevel;
        string name, category;

        cout << "\nEnter Item ID: ";
        cin >> id;
        cout << "Enter Item Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Category: ";
        getline(cin, category);
        cout << "Enter Quantity: ";
        cin >> quantity;
        cout << "Enter Reorder Level: ";
        cin >> reorderLevel;

        items.emplace_back(id, name, category, quantity, reorderLevel);
        cout << "Item added successfully!\n";
    }

    void updateStock() {
        int id, quantity;
        cout << "\nEnter Item ID to update stock: ";
        cin >> id;

        for (auto& item : items) {
            if (item.itemID == id) {
                cout << "Current Stock: " << item.quantity << endl;
                cout << "Enter New Quantity: ";
                cin >> quantity;
                item.quantity = quantity;
                cout << "Stock updated successfully!\n";
                return;
            }
        }
        cout << "Item not found!\n";
    }

    void viewInventory() const {
        if (items.empty()) {
            cout << "\nNo items in inventory.\n";
            return;
        }

        cout << "\nInventory:\n";
        cout << left << setw(10) << "Item ID"
             << setw(20) << "Name"
             << setw(15) << "Category"
             << setw(10) << "Quantity"
             << setw(15) << "Reorder Level" << endl;
        cout << string(70, '-') << endl;

        for (const auto& item : items) {
            item.display();
        }
    }

    void searchByName() const {
        string name;
        cout << "\nEnter Item Name to search: ";
        cin.ignore();
        getline(cin, name);

        bool found = false;
        for (const auto& item : items) {
            if (item.name == name) {
                cout << "\nItem Found:\n";
                item.display();
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Item not found!\n";
        }
    }

    void searchByCategory() const {
        string category;
        cout << "\nEnter Category to search: ";
        cin.ignore();
        getline(cin, category);

        bool found = false;
        for (const auto& item : items) {
            if (item.category == category) {
                if (!found) {
                    cout << "\nItems in Category:\n";
                    cout << left << setw(10) << "Item ID"
                         << setw(20) << "Name"
                         << setw(15) << "Category"
                         << setw(10) << "Quantity"
                         << setw(15) << "Reorder Level" << endl;
                    cout << string(70, '-') << endl;
                }
                item.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No items found in this category!\n";
        }
    }

    void generateAlerts() const {
        bool alert = false;
        cout << "\nLow Stock Alerts:\n";

        for (const auto& item : items) {
            if (item.needsRestock()) {
                if (!alert) {
                    cout << left << setw(10) << "Item ID"
                         << setw(20) << "Name"
                         << setw(15) << "Category"
                         << setw(10) << "Quantity"
                         << setw(15) << "Reorder Level" << endl;
                    cout << string(70, '-') << endl;
                }
                item.display();
                alert = true;
            }
        }
        if (!alert) {
            cout << "All items are sufficiently stocked.\n";
        }
    }

    void exportData() const {
        ofstream file("inventory_report.txt");
        if (!file) {
            cout << "Error creating file!\n";
            return;
        }

        file << left << setw(10) << "Item ID"
             << setw(20) << "Name"
             << setw(15) << "Category"
             << setw(10) << "Quantity"
             << setw(15) << "Reorder Level" << endl;
        file << string(70, '-') << endl;

        for (const auto& item : items) {
            file << left << setw(10) << item.itemID
                 << setw(20) << item.name
                 << setw(15) << item.category
                 << setw(10) << item.quantity
                 << setw(15) << item.reorderLevel << endl;
        }

        file.close();
        cout << "Inventory data exported to 'inventory_report.txt'.\n";
    }
};

// Main function
int main() {
    InventorySystem inventory;
    int choice;

    do {
        cout << "\n--- Warehouse Inventory Management System ---\n";
        cout << "1. Add New Item\n";
        cout << "2. Update Stock Level\n";
        cout << "3. View Inventory\n";
        cout << "4. Search Item by Name\n";
        cout << "5. Search Items by Category\n";
        cout << "6. Generate Low Stock Alerts\n";
        cout << "7. Export Inventory Data\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: inventory.addItem(); break;
            case 2: inventory.updateStock(); break;
            case 3: inventory.viewInventory(); break;
            case 4: inventory.searchByName(); break;
            case 5: inventory.searchByCategory(); break;
            case 6: inventory.generateAlerts(); break;
            case 7: inventory.exportData(); break;
            case 8: cout << "Exiting the system. Goodbye!\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 8);

    return 0;
}
