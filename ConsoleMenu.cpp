#include "ConsoleMenu.h"

#include <iostream>
#include <limits>

#include "Laptop.h"
#include "Phone.h"
#include "DesktopComputer.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

// ===================== INPUT HELPERS =====================

static void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ===================== CONSOLE COLORS (Windows) =====================

#ifdef _WIN32
static WORD g_defaultAttr = 0;
static bool g_inited = false;

static void initConsoleColors()
{
    if (g_inited) return;
    g_inited = true;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(h, &info))
        g_defaultAttr = info.wAttributes;
    else
        g_defaultAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
}

static void setColor(WORD attr)
{
    initConsoleColors();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
}

static void resetColor()
{
    initConsoleColors();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), g_defaultAttr);
}
#else
static void setColor(int) {}
static void resetColor() {}
#endif

// Color presets 
static void printTitle(const std::string& text)
{
#ifdef _WIN32
    setColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); 
#endif
    std::cout << "\n========================================\n";
    std::cout << text << "\n";
    std::cout << "========================================\n";
    resetColor();
}

static void printOk(const std::string& text)
{
#ifdef _WIN32
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif
    std::cout << "[OK] " << text << "\n";
    resetColor();
}

static void printError(const std::string& text)
{
#ifdef _WIN32
    setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif
    std::cout << "[ERROR] " << text << "\n";
    resetColor();
}

static void printInfo(const std::string& text)
{
#ifdef _WIN32
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); 
#endif
    std::cout << text << "\n";
    resetColor();
}

// ===================== PRODUCT PRINT =====================

static void printProductDetailed(const Inventory& inv, const std::shared_ptr<Product>& p)
{
    if (!p) return;

    const Category* cat = inv.getCategoryById(p->getCategoryId());
    std::string catName = cat ? cat->getName() : "Unknown";

#ifdef _WIN32
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#endif
    std::cout << "----------------------------------------\n";
#ifdef _WIN32
    resetColor();
#endif

    std::cout << " Serial   : " << p->getSerialNumber() << "\n";
    std::cout << " Type     : " << p->getType() << "\n";
    std::cout << " Name     : " << p->getName() << "\n";
    std::cout << " Brand    : " << p->getBrand() << "\n";
    std::cout << " Price    : " << p->getPrice() << "\n";
    std::cout << " Quantity : " << p->getQuantity() << "\n";
    std::cout << " Category : " << catName << " (ID=" << p->getCategoryId() << ")\n";

#ifdef _WIN32
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#endif
    std::cout << "----------------------------------------\n";
    resetColor();
}

// ===================== CATEGORY CHOOSER =====================

static int chooseCategoryId(const Inventory& inv)
{
    const auto& cats = inv.getCategories();
    if (cats.empty())
    {
        printError("No categories available.");
        return -1;
    }

    printTitle("CHOOSE CATEGORY");
    for (size_t i = 0; i < cats.size(); i++)
        std::cout << (i + 1) << ". " << cats[i].getName() << "\n";

    std::cout << "Choice (1-" << cats.size() << "): ";
    int choice;
    std::cin >> choice;

    if (std::cin.fail())
    {
        clearInput();
        printError("Invalid input.");
        return -1;
    }

    clearInput();

    if (choice < 1 || choice >(int)cats.size())
    {
        printError("Invalid category choice.");
        return -1;
    }

    return cats[choice - 1].getId();
}

// ===================== CTOR =====================

ConsoleMenu::ConsoleMenu(Inventory& inventory)
    : inventory(inventory)
{
#ifdef _WIN32
    initConsoleColors();
#endif
}

// ===================== MENU =====================

void ConsoleMenu::showMenu() const
{
    printTitle("TECH WAREHOUSE");

    std::cout << "1.  Add product\n";
    std::cout << "2.  List all products\n";
    std::cout << "3.  Search product by NAME\n";
    std::cout << "4.  Search product by SERIAL\n";
    std::cout << "5.  Edit product\n";
    std::cout << "6.  Remove product\n";
    std::cout << "7.  List products by CATEGORY\n";
    std::cout << "8.  Stock IN\n";
    std::cout << "9.  Stock OUT\n";
    std::cout << "10. List categories\n";
    std::cout << "11. Save data\n";
    std::cout << "0.  Exit\n";

#ifdef _WIN32
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); 
#endif
    std::cout << "Choice: ";
    resetColor();
}

void ConsoleMenu::run()
{
    int choice = -1;

    while (choice != 0)
    {
        showMenu();
        std::cin >> choice;

        if (std::cin.fail())
        {
            clearInput();
            printError("Invalid input.");
            continue;
        }

        clearInput();

        switch (choice)
        {
        case 1: addProduct(); break;
        case 2: listProducts(); break;
        case 3: searchByName(); break;
        case 4: searchBySerial(); break;
        case 5: editProduct(); break;
        case 6: removeProduct(); break;
        case 7: listByCategory(); break;
        case 8: stockIn(); break;
        case 9: stockOut(); break;
        case 10: listCategories(); break;
        case 11: saveData(); break;
        case 0: printOk("Exiting..."); break;
        default: printError("Unknown option."); break;
        }
    }
}

// ===================== CATEGORIES =====================

void ConsoleMenu::listCategories()
{
    const auto& cats = inventory.getCategories();
    if (cats.empty())
    {
        printError("No categories.");
        return;
    }

    printTitle("CATEGORIES");
    for (const auto& c : cats)
        std::cout << c.getId() << " | " << c.getName() << "\n";
}

// ===================== PRODUCTS =====================

void ConsoleMenu::listProducts()
{
    const auto& products = inventory.getAllProducts();
    if (products.empty())
    {
        printInfo("No products.");
        return;
    }

    printTitle("ALL PRODUCTS");
    for (const auto& p : products)
        printProductDetailed(inventory, p);
}

void ConsoleMenu::addProduct()
{
    
    std::string serial, name, brand;
    double price;
    int quantity;

    printTitle("ADD PRODUCT");

    std::cout << "Serial number: ";
    std::getline(std::cin, serial);

    if (serial.empty())
    {
        printError("Serial number cannot be empty.");
        return;
    }

    if (inventory.findBySerial(serial))
    {
        printError("Product with this serial already exists.");
        return;
    }

    std::cout << "Name: ";
    std::getline(std::cin, name);
    if (name.empty())
    {
        printError("Name cannot be empty.");
        return;
    }

    std::cout << "Brand: ";
    std::getline(std::cin, brand);

    std::cout << "Price: ";
    std::cin >> price;
    if (std::cin.fail() || price <= 0)
    {
        clearInput();
        printError("Invalid price.");
        return;
    }

    std::cout << "Quantity: ";
    std::cin >> quantity;
    if (std::cin.fail() || quantity < 0)
    {
        clearInput();
        printError("Invalid quantity.");
        return;
    }
    clearInput();

    // Choose category once
    int categoryId = chooseCategoryId(inventory);
    if (categoryId == -1) return;

    const Category* cat = inventory.getCategoryById(categoryId);
    std::string catName = cat ? cat->getName() : "";

    std::shared_ptr<Product> product;

    // Category determines type (Laptop / Phone / Desktop)
    if (categoryId == 1 || catName == "Laptop")
    {
        printInfo("Selected: Laptop (type is set automatically)");

        std::string cpu;
        int ram, storage;

        std::cout << "CPU: ";
        std::getline(std::cin, cpu);

        std::cout << "RAM (GB): ";
        std::cin >> ram;
        std::cout << "Storage (GB): ";
        std::cin >> storage;

        if (std::cin.fail() || ram <= 0 || storage <= 0)
        {
            clearInput();
            printError("Invalid laptop specs.");
            return;
        }
        clearInput();

        product = std::make_shared<Laptop>(serial, name, brand, price, quantity, categoryId, cpu, ram, storage);
    }
    else if (categoryId == 2 || catName == "Phone")
    {
        printInfo("Selected: Phone (type is set automatically)");

        std::string cpu;
        int storage;
        int has5g;

        std::cout << "CPU: ";
        std::getline(std::cin, cpu);

        std::cout << "Storage (GB): ";
        std::cin >> storage;
        std::cout << "5G support (1=yes,0=no): ";
        std::cin >> has5g;

        if (std::cin.fail() || storage <= 0 || (has5g != 0 && has5g != 1))
        {
            clearInput();
            printError("Invalid phone specs.");
            return;
        }
        clearInput();

        product = std::make_shared<Phone>(serial, name, brand, price, quantity, categoryId, cpu, storage, has5g == 1);
    }
    else if (categoryId == 3 || catName == "Desktop")
    {
        printInfo("Selected: Desktop (type is set automatically)");

        std::string cpu, gpu;
        int ram;

        std::cout << "CPU: ";
        std::getline(std::cin, cpu);
        std::cout << "GPU: ";
        std::getline(std::cin, gpu);

        std::cout << "RAM (GB): ";
        std::cin >> ram;

        if (std::cin.fail() || ram <= 0)
        {
            clearInput();
            printError("Invalid desktop specs.");
            return;
        }
        clearInput();

        product = std::make_shared<DesktopComputer>(serial, name, brand, price, quantity, categoryId, cpu, gpu, ram);
    }
    else
    {
        printError("Unknown category / cannot map category to product type.");
        return;
    }

    inventory.addProduct(product);
    printOk("Product added successfully.");
}

void ConsoleMenu::searchByName()
{
    std::string term;
    printTitle("SEARCH BY NAME");

    std::cout << "Name: ";
    std::getline(std::cin, term);

    auto results = inventory.searchByName(term);
    if (results.empty())
    {
        printInfo("No matches.");
        return;
    }

    for (const auto& p : results)
        printProductDetailed(inventory, p);
}

void ConsoleMenu::searchBySerial()
{
    std::string serial;
    printTitle("SEARCH BY SERIAL");

    std::cout << "Serial: ";
    std::getline(std::cin, serial);

    auto p = inventory.findBySerial(serial);
    if (!p)
    {
        printError("Product not found.");
        return;
    }

    printProductDetailed(inventory, p);
}

void ConsoleMenu::editProduct()
{
    std::string serial;
    printTitle("EDIT PRODUCT");

    std::cout << "Serial (product to edit): ";
    std::getline(std::cin, serial);

    auto p = inventory.findBySerial(serial);
    if (!p)
    {
        printError("Product not found.");
        return;
    }

    auto j = p->toJson();

    while (true)
    {
        printInfo("Current product:");
        printProductDetailed(inventory, p);

        std::cout << "\nEdit menu:\n";
        std::cout << "1. Serial number\n";
        std::cout << "2. Name\n";
        std::cout << "3. Brand\n";
        std::cout << "4. Price\n";
        std::cout << "5. Quantity\n";
        std::cout << "6. Category\n";

        if (p->getType() == "Laptop")
        {
            std::cout << "7. CPU\n8. RAM\n9. Storage\n";
        }
        else if (p->getType() == "Phone")
        {
            std::cout << "7. CPU\n8. Storage\n9. 5G\n";
        }
        else if (p->getType() == "DesktopComputer")
        {
            std::cout << "7. CPU\n8. GPU\n9. RAM\n";
        }

        std::cout << "0. Save & exit\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        if (std::cin.fail())
        {
            clearInput();
            printError("Invalid input.");
            continue;
        }
        clearInput();

        if (choice == 0) break;

        if (choice == 1)
        {
            std::string ns;
            std::cout << "New serial: ";
            std::getline(std::cin, ns);
            if (ns.empty()) { printError("Serial cannot be empty."); continue; }
            j["serialNumber"] = ns;
        }
        else if (choice == 2)
        {
            std::string v;
            std::cout << "New name: ";
            std::getline(std::cin, v);
            if (v.empty()) { printError("Name cannot be empty."); continue; }
            j["name"] = v;
        }
        else if (choice == 3)
        {
            std::string v;
            std::cout << "New brand: ";
            std::getline(std::cin, v);
            j["brand"] = v;
        }
        else if (choice == 4)
        {
            double v;
            std::cout << "New price: ";
            std::cin >> v;
            if (std::cin.fail() || v <= 0) { clearInput(); printError("Invalid price."); continue; }
            clearInput();
            j["price"] = v;
        }
        else if (choice == 5)
        {
            int v;
            std::cout << "New quantity: ";
            std::cin >> v;
            if (std::cin.fail() || v < 0) { clearInput(); printError("Invalid quantity."); continue; }
            clearInput();
            j["quantity"] = v;
        }
        else if (choice == 6)
        {
            int newCat = chooseCategoryId(inventory);
            if (newCat == -1) continue;
            j["categoryId"] = newCat;
        }
        else if (choice >= 7 && choice <= 9)
        {
            if (p->getType() == "Laptop")
            {
                if (choice == 7) { std::string v; std::cout << "New CPU: "; std::getline(std::cin, v); j["cpu"] = v; }
                if (choice == 8) { int v; std::cout << "New RAM: "; std::cin >> v; if (std::cin.fail() || v <= 0) { clearInput(); printError("Invalid RAM."); continue; } clearInput(); j["ram"] = v; }
                if (choice == 9) { int v; std::cout << "New Storage: "; std::cin >> v; if (std::cin.fail() || v <= 0) { clearInput(); printError("Invalid storage."); continue; } clearInput(); j["storage"] = v; }
            }
            else if (p->getType() == "Phone")
            {
                if (choice == 7) { std::string v; std::cout << "New CPU: "; std::getline(std::cin, v); j["cpu"] = v; }
                if (choice == 8) { int v; std::cout << "New Storage: "; std::cin >> v; if (std::cin.fail() || v <= 0) { clearInput(); printError("Invalid storage."); continue; } clearInput(); j["storage"] = v; }
                if (choice == 9) { int v; std::cout << "5G (1/0): "; std::cin >> v; if (std::cin.fail() || (v != 0 && v != 1)) { clearInput(); printError("Invalid 5G value."); continue; } clearInput(); j["has5g"] = (v == 1); }
            }
            else if (p->getType() == "DesktopComputer")
            {
                if (choice == 7) { std::string v; std::cout << "New CPU: "; std::getline(std::cin, v); j["cpu"] = v; }
                if (choice == 8) { std::string v; std::cout << "New GPU: "; std::getline(std::cin, v); j["gpu"] = v; }
                if (choice == 9) { int v; std::cout << "New RAM: "; std::cin >> v; if (std::cin.fail() || v <= 0) { clearInput(); printError("Invalid RAM."); continue; } clearInput(); j["ram"] = v; }
            }
        }
        else
        {
            printError("Invalid choice.");
        }
    }

    if (!inventory.updateProductFromJson(serial, j))
    {
        printError("Edit failed (duplicate serial or invalid data).");
        return;
    }

    std::string newSerial = j.contains("serialNumber") ? j["serialNumber"].get<std::string>() : serial;
    auto updated = inventory.findBySerial(newSerial);
    if (updated) p = updated;

    printOk("Product updated successfully.");
}

void ConsoleMenu::removeProduct()
{
    printTitle("REMOVE PRODUCT");

    std::string serial;
    std::cout << "Serial: ";
    std::getline(std::cin, serial);

    if (!inventory.removeProductBySerial(serial))
        printError("Not found.");
    else
        printOk("Removed.");
}

void ConsoleMenu::listByCategory()
{
    int categoryId = chooseCategoryId(inventory);
    if (categoryId == -1) return;

    auto products = inventory.listByCategory(categoryId);
    if (products.empty())
    {
        printInfo("No products in this category.");
        return;
    }

    printTitle("PRODUCTS BY CATEGORY");
    for (const auto& p : products)
        printProductDetailed(inventory, p);
}

// ===================== STOCK =====================

void ConsoleMenu::stockIn()
{
    printTitle("STOCK IN");

    std::string serial;
    int amount;

    std::cout << "Serial: ";
    std::getline(std::cin, serial);

    auto p = inventory.findBySerial(serial);
    if (!p)
    {
        printError("Invalid serial number (product not found).");
        return;
    }

    std::cout << "Amount: ";
    std::cin >> amount;
    if (std::cin.fail())
    {
        clearInput();
        printError("Invalid amount.");
        return;
    }
    clearInput();

    if (amount <= 0)
    {
        printError("Amount must be > 0.");
        return;
    }

    inventory.stockIn(serial, amount);
    printOk("Stock updated.");
    printInfo("New quantity: " + std::to_string(p->getQuantity()));
}

void ConsoleMenu::stockOut()
{
    printTitle("STOCK OUT");

    std::string serial;
    int amount;

    std::cout << "Serial: ";
    std::getline(std::cin, serial);

    auto p = inventory.findBySerial(serial);
    if (!p)
    {
        printError("Invalid serial number (product not found).");
        return;
    }

    std::cout << "Amount: ";
    std::cin >> amount;
    if (std::cin.fail())
    {
        clearInput();
        printError("Invalid amount.");
        return;
    }
    clearInput();

    if (amount <= 0)
    {
        printError("Amount must be > 0.");
        return;
    }

    if (amount > p->getQuantity())
    {
        printError("Not enough quantity.");
        printInfo("Available: " + std::to_string(p->getQuantity()));
        return;
    }

    inventory.stockOut(serial, amount);
    printOk("Stock updated.");
    printInfo("New quantity: " + std::to_string(p->getQuantity()));
}

// ===================== SAVE =====================

void ConsoleMenu::saveData()
{
    if (inventory.saveToFile("warehouse.json"))
        printOk("Data saved.");
    else
        printError("Save failed.");
}
