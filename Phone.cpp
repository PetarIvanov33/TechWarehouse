#include "Phone.h"

Phone::Phone(
    const std::string& serialNumber,
    const std::string& name,
    const std::string& brand,
    double price,
    int quantity,
    int categoryId,
    const std::string& cpu,
    int storageGB,
    bool has5G
)
    : Product(serialNumber, name, brand, price, quantity, categoryId),
      cpu(cpu),
      storageGB(storageGB),
      has5G(has5G) {}

std::string Phone::getType() const {
    return "Phone";
}

nlohmann::json Phone::toJson() const {
    return {
        {"type", getType()},
        {"serialNumber", serialNumber},
        {"name", name},
        {"brand", brand},
        {"price", price},
        {"quantity", quantity},
        {"categoryId", categoryId},
        {"cpu", cpu},
        {"storageGB", storageGB},
        {"has5G", has5G}
    };
}

Phone Phone::fromJson(const nlohmann::json& j) {
    return Phone(
        j.at("serialNumber").get<std::string>(),
        j.at("name").get<std::string>(),
        j.at("brand").get<std::string>(),
        j.at("price").get<double>(),
        j.at("quantity").get<int>(),
        j.at("categoryId").get<int>(),
        j.at("cpu").get<std::string>(),
        j.at("storageGB").get<int>(),
        j.at("has5G").get<bool>()
    );
}
