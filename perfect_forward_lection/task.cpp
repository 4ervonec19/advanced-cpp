#include <iostream>
#include <string_view>
#include <utility>
#include <chrono>

template <class T>
void insert_value(std::string_view& formatted_string, T&& value) { // Реализуем perfect_forward
    size_t position = formatted_string.find("{}"); // Ищем вхождение {}
    if (position == std::string_view::npos) { // Если не нашли
        throw std::runtime_error("No '{}' found!"); // Выбрасываем исключение
    }
    // Выводим подстроку + value
    std::cout << formatted_string.substr(0, position) << std::forward<T>(value); 
    // Убираем prefix + 2 ("{}")
    formatted_string.remove_prefix(position + 2);
}

// Базовый случай
void println(std::string_view formatted_string) {
    std::cout << formatted_string << std::endl;
}

// Рекурсивная функция
template <class T, class... Args>
void println(std::string_view formatted_string, T&& value, Args&&... args) {
    insert_value(formatted_string, std::forward<T>(value));
    println(formatted_string, std::forward<Args>(args)...);
}

int main() {
    auto res = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    println("{}, {}! Unix timestamp right now: {}", "Hello", "world", res);
    println("{}, {}! Unix timestamp right now: {}", "I", 404, res);
    println("{}, {}! Unix timestamp right now: {}", "I", 2.5, "Mr. X");
    println("{}, {}! Unix timestamp right now: {}", "", "", "");
    return 0;
}