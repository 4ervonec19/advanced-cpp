#include <iostream>
#include <type_traits>

template <typename T>
void printType(T&& param) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "Lvalue reference" << std::endl;
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        std::cout << "Rvalue reference" << std::endl;
    } else {
        std::cout << "Value" << std::endl;
    }
}

int main() {
    int x = 10;
    int& lref = x;
    int&& rref = 20;

    printType(x);       // Lvalue reference
    printType(lref);    // Lvalue reference
    printType(rref);    // Lvalue reference
    printType(30);      // Rvalue reference
    printType(std::move(x)); // Rvalue reference

    return 0;
}
