#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "lru_custom.hpp"

int main()
{
    size_t N, Size;
    std::cin >> N >> Size;
    std::cin.ignore(); // Игнорируем символ новой строки после ввода чисел

    LRU_Cache<std::string, std::vector<float>> lru_cache(N, Size);

    std::string line;
    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        std::string query;
        iss >> query;

        // Проверка на команду "print"
        if (query == "print")
        {
            lru_cache.print_cache(); // Вызов метода для печати кэша
            continue; // Переход к следующей итерации
        }

        // Проверка, является ли запрос обновлением (тип a)
        if (iss >> std::ws) // Если следующий элемент не пробел
        {
            std::vector<float> embedding(4);
            bool hasError = false; // Флаг для отслеживания ошибок
            size_t count = 0; // Счётчик успешно считанных значений

            for (float &f : embedding)
            {
                if (!(iss >> f))
                {
                    hasError = true; // Установить флаг об ошибке
                    break; // Прерывание цикла, если возникла ошибка
                }
                count++;
            }

            // Если не все значения были считаны, выводим ошибку
            if (count < embedding.size())
            {
                hasError = true; // Установить флаг об ошибке
            }

            if (hasError)
            {
                std::cout << "!STORERR!" << std::endl; // Вывод сообщения об ошибке
                continue; // Переход к следующей итерации цикла
            }

            // Попытка добавить в кэш
            if (lru_cache.add_note(query, embedding))
            {
                std::cout << "!STORED!" << std::endl;
            }
            else
            {
                std::cout << "!STORERR!" << std::endl; // Если не удалось сохранить
            }
        }
        else // Запрос на получение (тип б)
        {
            lru_cache.get(query);
        }
    }

    return 0;
}
