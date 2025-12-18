#pragma once

#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

#include "Category.h"
#include "Product.h"

class Inventory {
private:
    std::vector<Category> categories;
    std::vector<std::shared_ptr<Product>> products;

public:
    Inventory();

    // ---------- Categories ----------
    const std::vector<Category>& getCategories() const;
    const Category* getCategoryById(int id) const;

    // ---------- Products ----------
    void addProduct(const std::shared_ptr<Product>& product);

    bool serialExists(const std::string& serial) const;
    std::shared_ptr<Product> findBySerial(const std::string& serial) const;

    std::vector<std::shared_ptr<Product>> getAllProducts() const;
    std::vector<std::shared_ptr<Product>> searchByName(const std::string& term) const;
    std::vector<std::shared_ptr<Product>> listByCategory(int categoryId) const;

    bool removeProductBySerial(const std::string& serial);

    bool updateProductFromJson(const std::string& currentSerial, const nlohmann::json& updatedJson);

    // ---------- Validations / Stock ----------
    bool canStockIn(const std::string& serial, int amount) const;
    bool canStockOut(const std::string& serial, int amount) const;

    bool stockIn(const std::string& serial, int amount);
    bool stockOut(const std::string& serial, int amount);

    // ---------- Persistence ----------
    bool loadFromFile(const std::string& file);
    bool saveToFile(const std::string& file);
};
