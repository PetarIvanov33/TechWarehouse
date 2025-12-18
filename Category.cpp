#include "Category.h"

Category::Category(int id, const std::string& name, const std::string& description)
    : id(id), name(name), description(description) {
}

int Category::getId() const {
    return id;
}

const std::string& Category::getName() const {
    return name;
}

const std::string& Category::getDescription() const {
    return description;
}

Category Category::fromJson(const nlohmann::json& j) {
    return Category(
        j.at("id").get<int>(),
        j.at("name").get<std::string>(),
        j.at("description").get<std::string>()
    );
}

nlohmann::json Category::toJson() const {
    return {
        {"id", id},
        {"name", name},
        {"description", description}
    };
}
