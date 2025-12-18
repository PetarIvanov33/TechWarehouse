#include <iostream>
#include "Inventory.h"
#include "ConsoleMenu.h"

int main() {
    Inventory inventory;

    if (!inventory.loadFromFile("warehouse.json")) {
        std::cout << "Failed to load data file. Starting with empty inventory.\n";
    }

    ConsoleMenu menu(inventory);
    menu.run();

    if (!inventory.saveToFile("warehouse.json")) {
        std::cout << "Failed to save data file.\n";
    }

    return 0;
}
