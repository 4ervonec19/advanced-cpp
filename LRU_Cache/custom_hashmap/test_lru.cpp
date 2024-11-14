#include "lru.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Основная функция для запуска тестов
int main() {

  // if (!TestAddAndRetrieve()) return 1;
  // if (!TestAddBeyondCapacity()) return 1;
  // if (!TestUpdateEntry()) return 1;
  // if (!TestMemoryReleaseOnEviction()) return 1;

  LRU_Cache<std::string, std::vector<float>> cache(3, 1024);

  // Тест добавления элемента
  assert(cache.add_note("key1", std::vector<float>(4, 1.0f)));
  assert(cache.add_note("key1", std::vector<float>(4, 1.25f)));
  assert(cache.add_note("key2", std::vector<float>(4, 2.0f)));
  // // Проверка на размер кэша
  assert(cache.size() == 2);

  // // Проверка кодировки ключа
  assert(cache.add_note("ключ3", std::vector<float>(4, 3.0f)));
  assert(cache.size() == 3);

  // // Добавляем четвертый
  assert(cache.add_note("key4", std::vector<float>(4, 3.0f)));
  // // Проверяем удаление первого
  std::cout << "\nExpected output:\n !NOEMBED! " << std::endl;
  std::cout << "\nActual output:\n ";
  cache.get("key1");
  assert(cache.size() == 3);
  std::cout << std::endl;

  // // Проверка обновления
  assert(cache.add_note("key2", std::vector<float>(4, 10.0f)));
  assert(cache.size() == 3);

  // // Проверка clear
  cache.clear();
  assert(cache.size() == 0);

  // // Проверка на работу с памятью
  assert(cache.add_note("test1", std::vector<float>(64, 15.0f)));
  assert(cache.add_note("test2", std::vector<float>(64, 20.0f)));
  assert(cache.add_note("test3", std::vector<float>(64, 25.0f)));

  assert(cache.size() == 3);
  size_t initial_size_bytes = cache.size_bytes();

  // // Добавляем четвертый элемент, который должен вызвать удаление "test1"
  assert(cache.add_note("test4", std::vector<float>(64, 30.0f)));

  // // Ожидается !NOEMBED! для теста 1
  std::cout << "\nExpected output for key1:\n !NOEMBED! " << std::endl;
  std::cout << "\nActual output:\n ";
  cache.get("key1");
  std::cout << std::endl;
  assert(cache.size() == 3); // Проверяем, что размер кэша остался 3

  assert(cache.size_bytes() ==
         783); // Проверяем, что общий размер в байтах не увеличился

  cache.clear();
  assert(cache.size() == 0);

  assert(cache.add_note("test1", std::vector<float>(4, 15.0f)));
  assert(cache.add_note("test2", std::vector<float>(4, 20.0f)));
  assert(cache.add_note("test3", std::vector<float>(4, 25.0f)));

  // Проверка на добавление изначально бОльшей, чем размер записи
  assert(!cache.add_note("aaaaaaaaaa", std::vector<float>(256, 30.0f)));
  assert(cache.size() == 3);

  cache.print_cache();

  std::cout << "All tests passed successfully!" << std::endl;
  return 0;
}
