#### Команды для ввода и проверки работы задания

```bash
cd LRU_Cache # Переход в LRU_Cache
cd custom_hashmap # Переход в custom_hashmap
find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i # Перевод в формат
clang-format --dry-run --Werror --style=file *.cpp *.hpp # Проверка формата
g++ --coverage -g -O0 -fsanitize=address -std=c++20 \
    -Wall -Werror -Wextra test_lru.cpp -o test_lru 
./test_lru # Запуска test_lru с покрытием
gcov -o . test_lru # Позволит посмотреть процент покрытия lru.hpp
find . -type f \( -name "*.gcov" -o -name "*.gcda" \) -delete # Очищаем все ненужное
g++ --coverage -g -O0 -fsanitize=address -std=c++20 \
    -Wall -Werror -Wextra test_hashtable.cpp -o test_hashtable
./test_hashtable # Запуск с покрытием для test_hashtable
gcov -o . test_hashtable # Позволит посмотреть процент покрытия hashtable.hpp
find . -type f \( -name "*.gcov" -o -name "*.gcda" \) -delete # удаляем все лишнее
g++ -std=c++20 -O2 -fsanitize=address \
    -Wall -Werror -Wextra main.cpp -o main 
./main # Запуск main запускаем main (float dim = 64).
```