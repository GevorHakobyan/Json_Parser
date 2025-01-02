#include "Json.hpp"

int main() {
    cjs::Json ob{"fd", 5, true};
    std::cout << ob.is_array() << "\n";
    ob.push_back("fdff");
    std::cout << ob[3] << "\n";
    return 0;
}