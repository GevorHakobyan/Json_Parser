#include "JsonFile.hpp"
#include <nlohmann/json.hpp>

int main() {
    nlohmann::json ob{"fd", 5};
    std::cout << ob.is_array() << "\n";
    std::cout << ob[0] << "\n";
    return 0;
}