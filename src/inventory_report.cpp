#include "inventory_report.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

bool isValidQuantity(int quantity) {
    return (quantity >= 0.0);
}

bool isValidPrice(double price) {
    return ( price >= 0.0);
}

double calculateItemValue(const InventoryItem& item) {
    if (!isValidQuantity(item.quantity) || !isValidPrice(item.price))
        return 0.0;

    return item.quantity * item.price; 
}

int readInventoryFile(string filename, InventoryItem items[], int maxItems) {
    if (items == nullptr || maxItems < 0)
        return 0;

    ifstream in(filename);
    if (!in.is_open())
        return 0;

    int count {0};
    InventoryItem item;

    while (count < maxItems &&
           in >> item.sku 
              >> item.name
              >> item.quantity
              >> item.price) {
        if (isValidQuantity(item.quantity) && isValidPrice(item.price)) {
            items[count] = item;
            count++;
        }
     }

    in.close();
    return count;
}

bool writeInventoryReport(string filename, const InventoryItem items[], int count) {
    if (items == nullptr || count < 0)
        return false;

    ofstream out(filename);
    if (!out.is_open())
        return false;

    out << fixed << setprecision(2);
    out << "Inventory Report" << endl;
    out << "SKU Name Quantity Price Value" << endl;

    for (int i = 0; i < count; i++) {
        out << items[i].sku << " "
            << items[i].name << " "
            << items[i].quantity << " "
            << items[i].price << " "
            << calculateItemValue(items[i])
            << endl;
    }
    
    out << "Total inventory value: "
        << calculateTotalInventoryValue(items, count)
        << endl;

    out.close(); 
    return true;
}

double calculateTotalInventoryValue(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0)
        return 0.0;

    double total {0.0};
    for (int i = 0; i < count; i++)
        total += calculateItemValue(items[i]);

    return total;
}

int findItemBySku(const InventoryItem items[], int count, string sku) {
    if (items == nullptr || count <= 0)
        return -1;

    for (int i = 0; i < count; i++) {
        if(items[i].sku == sku)
            return i;
    }

    return -1;
}

int findHighestValueItemIndex(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0)
        return -1;  

    int highestIndex {0};
    double highestValue = calculateItemValue(items[0]);

    for (int i = 1; i < count; i++) {
        double currentValue = calculateItemValue(items[i]);
        if (currentValue > highestValue) {
            highestValue = currentValue;
            highestIndex = i;
        }
    }
    
    return highestIndex;
}
