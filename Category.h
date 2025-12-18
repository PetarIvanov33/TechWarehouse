#pragma once

#include <string>
#include <nlohmann/json.hpp>

class Category {
private:
    int id;
    std::string name;
    std::string description;

public:
    Category() = default;
    Category(int id, const std::string& name, const std::string& description);

    int getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;

    static Category fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;
};
#pragma once
