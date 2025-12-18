#include "Inventory.h"

#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

#include "Laptop.h"
#include "Phone.h"
#include "DesktopComputer.h"

using json = nlohmann::json;

static std::string getStringAny(const json& j, std::initializer_list<const char*> keys)
{
    for (auto k : keys)
    {
        if (j.contains(k) && j[k].is_string())
            return j[k].get<std::string>();
    }
    return "";
}

// ===================== CONSTRUCTOR =====================

Inventory::Inventory() {
    categories.emplace_back(1, "Laptop", "Portable computers");
    categories.emplace_back(2, "Phone", "Mobile phones");
    categories.emplace_back(3, "Desktop", "Desktop computers");
}

// ===================== CATEGORIES =====================

const std::vector<Category>& Inventory::getCategories() const {
    return categories;
}

const Category* Inventory::getCategoryById(int id) const {
    for (const auto& c : categories) {
        if (c.getId() == id)
            return &c;
    }
    return nullptr;
}

// ===================== PRODUCTS =====================

void Inventory::addProduct(const std::shared_ptr<Product>& product) {
    if (!product)
        return;

    if (serialExists(product->getSerialNumber()))
        return;

    products.push_back(product);
}

bool Inventory::serialExists(const std::string& serial) const {
    if (serial.empty())
        return false;

    for (const auto& p : products) {
        if (p->getSerialNumber() == serial)
            return true;
    }
    return false;
}

std::shared_ptr<Product> Inventory::findBySerial(const std::string& serial) const {
    if (serial.empty())
        return nullptr;

    for (const auto& p : products) {
        if (p->getSerialNumber() == serial)
            return p;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Product>> Inventory::getAllProducts() const {
    return products;
}

std::vector<std::shared_ptr<Product>> Inventory::searchByName(const std::string& term) const {
    std::vector<std::shared_ptr<Product>> result;

    if (term.empty())
        return result;

    for (const auto& p : products) {
        if (p->getName().find(term) != std::string::npos) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Product>> Inventory::listByCategory(int categoryId) const {
    std::vector<std::shared_ptr<Product>> result;

    if (!getCategoryById(categoryId))
        return result;

    for (const auto& p : products) {
        if (p->getCategoryId() == categoryId) {
            result.push_back(p);
        }
    }
    return result;
}

bool Inventory::removeProductBySerial(const std::string& serial) {
    if (serial.empty())
        return false;

    auto it = std::remove_if(
        products.begin(),
        products.end(),
        [&](const std::shared_ptr<Product>& p) {
            return p->getSerialNumber() == serial;
        }
    );

    if (it == products.end())
        return false;

    products.erase(it, products.end());
    return true;
}

bool Inventory::updateProductFromJson(const std::string& currentSerial, const json& updatedJson)
{
    auto existing = findBySerial(currentSerial);
    if (!existing)
        return false;

    std::string newSerial = getStringAny(updatedJson, { "serialNumber", "serial", "SerialNumber", "Serial" });
    if (newSerial.empty())
        newSerial = existing->getSerialNumber();

    if (newSerial != currentSerial && serialExists(newSerial))
        return false;

    std::string type = getStringAny(updatedJson, { "type", "Type" });
    if (type.empty())
        type = existing->getType();

    std::shared_ptr<Product> newP;

    try
    {
        if (type == "Laptop")
            newP = std::make_shared<Laptop>(Laptop::fromJson(updatedJson));
        else if (type == "Phone")
            newP = std::make_shared<Phone>(Phone::fromJson(updatedJson));
        else if (type == "DesktopComputer")
            newP = std::make_shared<DesktopComputer>(DesktopComputer::fromJson(updatedJson));
        else
            return false;
    }
    catch (...)
    {
        return false;
    }

    for (auto& p : products)
    {
        if (p && p->getSerialNumber() == currentSerial)
        {
            p = newP;
            return true;
        }
    }

    return false;
}

// ===================== VALIDATIONS =====================

bool Inventory::canStockIn(const std::string& serial, int amount) const {
    if (serial.empty())
        return false;

    if (amount <= 0)
        return false;

    auto product = findBySerial(serial);
    if (!product)
        return false;

    return true;
}

bool Inventory::canStockOut(const std::string& serial, int amount) const {
    if (serial.empty())
        return false;

    if (amount <= 0)
        return false;

    auto product = findBySerial(serial);
    if (!product)
        return false;

    if (amount > product->getQuantity())
        return false;

    return true;
}

// ===================== STOCK (DEFENSIVE) =====================

bool Inventory::stockIn(const std::string& serial, int amount) {
    if (!canStockIn(serial, amount))
        return false;

    auto product = findBySerial(serial);
    product->increaseQuantity(amount);
    return true;
}

bool Inventory::stockOut(const std::string& serial, int amount) {
    if (!canStockOut(serial, amount))
        return false;

    auto product = findBySerial(serial);
    return product->decreaseQuantity(amount);
}

// ===================== JSON =====================

bool Inventory::loadFromFile(const std::string& file) {
    std::ifstream in(file);
    if (!in.is_open())
        return false;

    json j;
    try {
        in >> j;
    }
    catch (...) {
        return false;
    }

    products.clear();

    if (!j.contains("products") || !j["products"].is_array())
        return true;

    for (const auto& item : j["products"]) {
        if (!item.contains("type"))
            continue;

        try {
            std::string type = item["type"].get<std::string>();

            if (type == "Laptop") {
                products.push_back(std::make_shared<Laptop>(Laptop::fromJson(item)));
            }
            else if (type == "Phone") {
                products.push_back(std::make_shared<Phone>(Phone::fromJson(item)));
            }
            else if (type == "DesktopComputer") {
                products.push_back(std::make_shared<DesktopComputer>(DesktopComputer::fromJson(item)));
            }
        }
        catch (...) {
            continue;
        }
    }

    return true;
}

bool Inventory::saveToFile(const std::string& file) {
    json j;
    j["products"] = json::array();

    for (const auto& p : products) {
        j["products"].push_back(p->toJson());
    }

    std::ofstream out(file);
    if (!out.is_open())
        return false;

    out << j.dump(4);
    return true;
}
