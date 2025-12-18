#pragma once

#include "Inventory.h"

class ConsoleMenu {
private:
    Inventory& inventory;

    // ---------- Core ----------
    void showMenu() const;

    // ---------- Products ----------
    void addProduct();
    void listProducts();
    void searchByName();
    void searchBySerial();
    void editProduct();
    void removeProduct();

    // ---------- Categories ----------
    void listCategories();
    void listByCategory();

    // ---------- Stock ----------
    void stockIn();
    void stockOut();

    // ---------- Persistence ----------
    void saveData();

public:
    explicit ConsoleMenu(Inventory& inventory);
    void run();
};
