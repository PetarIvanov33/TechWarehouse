#include "Laptop.h"

Laptop::Laptop(
    const std::string& serialNumber,
    const std::string& name,
    const std::string& brand,
    double price,
    int quantity,
    int categoryId,
    const std::string& cpu,
    int ramGB,
    int storageGB
)
    : Product(serialNumber, name, brand, price, quantity, categoryId),
    cpu(cpu),
    ramGB(ramGB),
    storageGB(storageGB) {
}

const std::string& Laptop::getCpu() const {
    return cpu;
}

int Laptop::getRamGB() const {
    return ramGB;
}

int Laptop::getStorageGB() const {
    return storageGB;
}

std::string Laptop::getType() const {
    return "Laptop";
}

nlohmann::json Laptop::toJson() const {
    return {
        {"type", getType()},
        {"serialNumber", serialNumber},
        {"name", name},
        {"brand", brand},
        {"price", price},
        {"quantity", quantity},
        {"categoryId", categoryId},
        {"cpu", cpu},
        {"ramGB", ramGB},
        {"storageGB", storageGB}
    };
}

Laptop Laptop::fromJson(const nlohmann::json& j) {
    return Laptop(
        j.at("serialNumber").get<std::string>(),
        j.at("name").get<std::string>(),
        j.at("brand").get<std::string>(),
        j.at("price").get<double>(),
        j.at("quantity").get<int>(),
        j.at("categoryId").get<int>(),
        j.at("cpu").get<std::string>(),
        j.at("ramGB").get<int>(),
        j.at("storageGB").get<int>()
    );
}
