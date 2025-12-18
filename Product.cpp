#include "Product.h"

Product::Product(
    const std::string& serialNumber,
    const std::string& name,
    const std::string& brand,
    double price,
    int quantity,
    int categoryId
)
    : serialNumber(serialNumber),
    name(name),
    brand(brand),
    price(price),
    quantity(quantity),
    categoryId(categoryId) {
}

const std::string& Product::getSerialNumber() const { return serialNumber; }
const std::string& Product::getName() const { return name; }
const std::string& Product::getBrand() const { return brand; }
double Product::getPrice() const { return price; }
int Product::getQuantity() const { return quantity; }
int Product::getCategoryId() const { return categoryId; }


void Product::setName(const std::string& newName) {
    name = newName;
}

void Product::setBrand(const std::string& newBrand) {
    brand = newBrand;
}

void Product::setPrice(double newPrice) {
    if (newPrice >= 0)
        price = newPrice;
}

void Product::setQuantity(int newQuantity) {
    if (newQuantity >= 0)
        quantity = newQuantity;
}

void Product::increaseQuantity(int amount) {
    if (amount > 0)
        quantity += amount;
}

bool Product::decreaseQuantity(int amount) {
    if (amount <= 0 || amount > quantity)
        return false;
    quantity -= amount;
    return true;
}
