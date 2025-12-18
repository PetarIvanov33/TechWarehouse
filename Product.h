#pragma once

#include <string>
#include <nlohmann/json.hpp>

class Product {
protected:
    std::string serialNumber;
    std::string name;
    std::string brand;
    double price;
    int quantity;
    int categoryId;

public:
    Product(
        const std::string& serialNumber,
        const std::string& name,
        const std::string& brand,
        double price,
        int quantity,
        int categoryId
    );

    virtual ~Product() = default;

    // Getters
    const std::string& getSerialNumber() const;
    const std::string& getName() const;
    const std::string& getBrand() const;
    double getPrice() const;
    int getQuantity() const;
    int getCategoryId() const;

    // Setters  
    void setName(const std::string& name);
    void setBrand(const std::string& brand);
    void setPrice(double price);
    void setQuantity(int quantity);

    void increaseQuantity(int amount);
    bool decreaseQuantity(int amount);

    virtual std::string getType() const = 0;
    virtual nlohmann::json toJson() const = 0;
};
