#include "Product.h"
#include <string>

class DesktopComputer : public Product {
private:
    std::string cpu;
    std::string gpu;
    int ramGB;

public:
    DesktopComputer(
        const std::string& serialNumber,
        const std::string& name,
        const std::string& brand,
        double price,
        int quantity,
        int categoryId,
        const std::string& cpu,
        const std::string& gpu,
        int ramGB
    );

    std::string getType() const override;
    nlohmann::json toJson() const override;

    static DesktopComputer fromJson(const nlohmann::json& j);
};
#pragma once
