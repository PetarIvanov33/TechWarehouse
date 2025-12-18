#pragma once

#include "Product.h"
#include <string>

class Phone : public Product {
private:
    std::string cpu;
    int storageGB;
    bool has5G;

public:
    Phone(
        const std::string& serialNumber,
        const std::string& name,
        const std::string& brand,
        double price,
        int quantity,
        int categoryId,
        const std::string& cpu,
        int storageGB,
        bool has5G
    );

    std::string getType() const override;
    nlohmann::json toJson() const override;

    static Phone fromJson(const nlohmann::json& j);
};
