#include "lru.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Тестирование LRU-кэша
void testLRU_Cache() {

  // Инициализируем LRU-кэш размера 3 и памяти 1024 байта
  LRU_Cache<std::string, std::vector<float>> cache(3, 1024);
  // Проверяем размеры (нулевые, ничего не добавляли)
  assert(cache.size() == 0);
  assert(cache.size_bytes() == 0);

  // Проверяем добавление элемента
  assert(cache.add_note(
      "key1",
      std::vector<float>(4, 1.0f))); // Добавляем массив 1.0f по ключу "key1"
  assert(cache.add_note(
      "key1", std::vector<float>(4, 1.25f))); // Добавляем массив 1.25f по ключу
                                              // "key1" (Обновляем по сути)
  assert(cache.add_note(
      "key2",
      std::vector<float>(4, 2.0f))); // Добавляем массив 2.0f по ключу "key2"
  assert(cache.size() ==
         2); // Проверяем, что размер кэша остался 2 несмотря на 3 операции
  assert(cache.size_bytes() ==
         40); // float = 4 байта (итого: 4*4(размер массива)*2(кол-во записей))
              // + 4(размер стрингов)*2(кол-во записей)

  // Проверка кодировки ключа
  assert(cache.add_note("ключ3", std::vector<float>(4, 3.0f)));
  assert(cache.size() == 3);

  // Добавляем четвертый
  assert(cache.add_note(
      "key4",
      std::vector<float>(4, 3.0f))); // Добавляем массив 3.0f по ключу "key4"
  // Должно произойти удаление первого, как самого старого в кэше
  assert(!cache.get("key1")); // Должен быть false
  assert(cache.size() == 3);  // Размер оставлся равный трём

  // Проверка обновления
  assert(cache.add_note(
      "key2",
      std::vector<float>(4, 10.0f))); // Обновляем значение по ключу "key2"
  assert(cache.size() == 3);          // Размер остался равен 3

  // Проверка get() на true
  assert(cache.get("ключ3"));

  cache.print_cache();

  // // Проверка clear
  cache.clear();
  assert(cache.size() == 0);

  // Обработка 3-x и 4-x байтовых символов:
  cache.add_note("улыбка 😀", std::vector<float>(4, 10.0f));
  cache.add_note("Евронский €", std::vector<float>(4, 10.0f));

  std::cout << "All LRU_Cache tests passed successfully!" << std::endl;
}

void testLRU_CacheMemory() {

  // Инициализируем LRU-кэш размера 3 и памяти 1024 байта
  LRU_Cache<std::string, std::vector<float>> cache(3, 1024);
  // Проверяем размеры (нулевые, ничего не добавляли)
  assert(cache.size() == 0);
  assert(cache.size_bytes() == 0);

  // // Проверка на работу с памятью
  assert(cache.add_note("test1", std::vector<float>(64, 15.0f)));
  assert(cache.add_note("test2", std::vector<float>(64, 20.0f)));
  assert(cache.add_note("test3", std::vector<float>(64, 25.0f)));

  // Размер должен стать 3
  assert(cache.size() == 3);

  // // Добавляем четвертый элемент, который должен вызвать удаление "test1"
  assert(cache.add_note("test4", std::vector<float>(64, 30.0f)));
  assert(cache.size_bytes() ==
         783); // Проверяем, что общий размер в байтах не увеличился

  // Проверка на добавление изначально бОльшей, чем размер записи
  assert(!cache.add_note("aaaaaaaaaa", std::vector<float>(256, 30.0f)));
  assert(cache.size() == 3);
  std::cout << "All LRU_Cache Memory tests passed successfully!" << std::endl;
}

// Для покрытия тестируем хэш-таблицу
void testHashTable() {

  // Инициализиурем с размером 4
  LRU_Cache<std::string, std::vector<float>> cache(8, 8192);
  // Проверяем размеры (нулевые, ничего не добавляли)
  assert(cache.size() == 0);
  assert(cache.size_bytes() == 0);

  // Должен сработать rehash() внутри hashtable
  assert(cache.add_note("test1", std::vector<float>(64, 15.0f)));
  assert(cache.add_note("test2", std::vector<float>(64, 20.0f)));
  assert(cache.add_note("test3", std::vector<float>(64, 25.0f)));
  assert(cache.add_note("test4", std::vector<float>(64, 35.0f)));
  assert(cache.add_note("test5", std::vector<float>(64, 45.0f)));
  assert(cache.add_note("test6", std::vector<float>(64, 55.0f)));
  assert(cache.add_note("test7", std::vector<float>(64, 65.0f)));
  assert(cache.add_note("test8", std::vector<float>(64, 75.0f)));

  std::cout << "All HashTable tests passed successfully" << std::endl;
}

// Основная функция для запуска тестов
int main() {
  testLRU_Cache();
  testLRU_CacheMemory();
  testHashTable();
  return 0;
}
