#pragma once

#include "Product.h"
#include <string>

class Laptop : public Product {
private:
    std::string cpu;
    int ramGB;
    int storageGB;

public:
    Laptop(
        const std::string& serialNumber,
        const std::string& name,
        const std::string& brand,
        double price,
        int quantity,
        int categoryId,
        const std::string& cpu,
        int ramGB,
        int storageGB
    );

    const std::string& getCpu() const;
    int getRamGB() const;
    int getStorageGB() const;

    std::string getType() const override;
    nlohmann::json toJson() const override;

    static Laptop fromJson(const nlohmann::json& j);
};
