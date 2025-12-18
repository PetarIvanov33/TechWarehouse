#include "DesktopComputer.h"

DesktopComputer::DesktopComputer(
    const std::string& serialNumber,
    const std::string& name,
    const std::string& brand,
    double price,
    int quantity,
    int categoryId,
    const std::string& cpu,
    const std::string& gpu,
    int ramGB
)
    : Product(serialNumber, name, brand, price, quantity, categoryId),
    cpu(cpu),
    gpu(gpu),
    ramGB(ramGB) {
}

std::string DesktopComputer::getType() const {
    return "DesktopComputer";
}

nlohmann::json DesktopComputer::toJson() const {
    return {
        {"type", getType()},
        {"serialNumber", serialNumber},
        {"name", name},
        {"brand", brand},
        {"price", price},
        {"quantity", quantity},
        {"categoryId", categoryId},
        {"cpu", cpu},
        {"gpu", gpu},
        {"ramGB", ramGB}
    };
}

DesktopComputer DesktopComputer::fromJson(const nlohmann::json& j) {
    return DesktopComputer(
        j.at("serialNumber").get<std::string>(),
        j.at("name").get<std::string>(),
        j.at("brand").get<std::string>(),
        j.at("price").get<double>(),
        j.at("quantity").get<int>(),
        j.at("categoryId").get<int>(),
        j.at("cpu").get<std::string>(),
        j.at("gpu").get<std::string>(),
        j.at("ramGB").get<int>()
    );
}
