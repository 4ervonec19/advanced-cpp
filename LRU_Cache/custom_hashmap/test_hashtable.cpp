#include "hashtable.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <string>

// Структура hasher для обработки коллизий
struct SimpleStringHasher {
  std::size_t operator()(const std::string &str) const {
    std::size_t hash = 0;
    for (char c : str) {
      hash += static_cast<std::size_t>(c);
    }
    return hash;
  }
};

void testHashTableOperations() {
  // Тестируем основные опреации custom хэш-таблицы

  // Объявляем хэш-таблицу, выделив для Bucket lists размер 4:
  CustomUnorderedMap<std::string, float> map(4);
  assert(
      map.size() ==
      0); // Текущий размер должен быть равен 0, так как еще ничего не заполняли
  assert(map.load_factor() == static_cast<float>(0)); // Должен быть 0

  // Тест insert ()
  // Добавили значения 2.4 и 3.5 по ключам "test1" и "test2" соответственно
  assert(map.insert("test1", 2.4)); // Должно получиться true
  assert(map.insert("test2", 3.5)); //  Должно получиться true
  assert(map.size() == 2);          // Размер должен стать 2
  float current_factor =
      map.load_factor(); // Получаем текущий коэффициент заполнения
  assert(current_factor == static_cast<float>(0.5)); // Должен быть 0.5 (2 / 4)

  assert(map.insert("test2", 4.5)); // Обновляем значение по ключу 2
  assert(map.size() == 2);          // Размер должен остаться 2
  float current_factor_ =
      map.load_factor(); // Получаем текущий коэффициент заполнения
  assert(current_factor_ ==
         static_cast<float>(0.5)); // Должен остаться 0.5 (2 / 4)

  // Тест find()
  auto iter = map.find("test2"); // Получаем итератор на ключ "test2"
  assert(iter !=
         map.end()); // Подтверждаем, что итератор не на конец контейнера
  assert((*iter).first == "test2"); // Ключ равен test2
  assert((*iter).second ==
         static_cast<float>(4.5)); // Значение равно 4.5 (на что мы поменяли)
  assert(map.size() ==
         2); // Размер должен остатьтся 2 (Внезапных удалений не произошло)

  // Тест rehash()
  assert(map.insert(
      "test3",
      12.5)); // Должно корректно вставаиться и под капотом произойдет rehash
  assert(map.load_factor() ==
         static_cast<float>(0.375)); // Должен произойти rehash и коэффициент
                                     // заполнения должен стать 0.375 (3 / 8)
  assert(map.size() == 3);           // Размер должен увеличиться до трёх

  // Тест reserve()
  map.reserve(16); // Резервируем новое число ячеек и делаем rehash
  assert(map.load_factor() ==
         static_cast<float>(
             0.1875)); // Коэффициент заполнения должен стать 0.1875 (3 / 16)
  assert(map.size() == 3); // Размер остаться равным 3

  // Тест empty()
  assert(!map.empty()); // Не должен быть пустым

  // Тест erase()
  assert(
      map.erase("test2")); // Удаляем по ключу "test2", должно получиться true
  assert(map.size() == 2); // Размер сократили; должен стать 2
  assert(map.load_factor() ==
         static_cast<float>(
             0.125)); // Коэффициент заполнения должен стать 0.125 (2 / 16)
  auto iter_after_erase = map.find("test2"); // Попытка поиска
  assert(iter_after_erase == map.end());     // Должны быть итератор на конец

  // Тест оператора []
  float value = map["test1"]; //  Должен вернуть значение по ключу "test1"
  assert(value == static_cast<float>(2.4)); // Должно совпасть с 2.4
  assert(map.size() == 2);                  // Размер не поменялся, остался 2

  // Тест оператора [], если ключа нет в таблице:
  float new_value = map["test4"];    // Получаем по ключу
  auto iterator = map.find("test4"); // Должен найти
  assert(iterator != map.end());     // Итератор не на конец контейнера
  assert(map.load_factor() == static_cast<float>(0.1875)); // Добавился элемент
  assert(map.size() == 3);                                 // Размер 3
  assert(new_value == 0);                                  // Нулевое значение

  map["test5"] = 22.5;                // Должен сработать
  float new_value_ = map["test5"];    // Получаем по ключу
  auto iterator_ = map.find("test5"); // Должен найти
  assert(iterator_ != map.end());     // Итератор не на конец контейнера
  assert(map.load_factor() == static_cast<float>(0.25)); // Добавился элемент
  assert(map.size() == 4);                               // Размер 3
  assert(new_value_ == 22.5);                            // Значение по ключу

  // Тест count()
  size_t key_count = map.count("test5");     // count() по существущему ключу
  assert(key_count == 1);                    // Должен найти
  size_t new_key_count = map.count("test6"); // count() по несуществующему ключу
  assert(new_key_count == 0);                // Не должен найти

  // Тест clear()
  map.clear();             // Очищаем hashmap
  assert(map.size() == 0); // Ничего не осталось
  assert(map.load_factor() ==
         static_cast<float>(0.0)); // Коэффициент заполнения равен 0

  // Тест удаления того, чего нет
  assert(!map.erase("test10")); // Должны получить false

  std::cout << "All HashTable operations tests passed successfully!"
            << std::endl;
}

void testHashTableIterator() {
  // Тестируем custom Iterator для хэш-таблицы

  // Объявляем хэш-таблицу, выделив для Bucket lists размер 4:
  CustomUnorderedMap<std::string, float> map(4);

  // Заполняем хэш-таблицу экспериментальными значениями
  map["apple"] = 0.25;
  map["watermelon"] = 1.24;
  map["tomato"] = 0.125;

  // Тестируем итератор на start
  auto start_iterator =
      map.begin(); // Инициализируем итератор на начало таблицы
  assert(start_iterator != map.end()); // Проверяем его на неравенство end
                                       // (также проверка оператора !=)

  // Тестируем оператор ++
  auto second_iterator = ++start_iterator;
  assert(second_iterator != map.end()); // Проверяем его на неравенство end
                                        //
  // Проверка итерирумости хэш-таблицы:
  for (auto iterator = map.begin(); iterator != map.end(); ++iterator) {
    auto key_val = *iterator;
  }

  // Итератор на 3 элемент
  auto third_iterator = ++second_iterator;
  assert(third_iterator != map.end());

  // Итератор на конец
  auto final_iterator = ++third_iterator;
  assert(final_iterator == map.end());

  // Тестируем автоматический итератор на конец, если пусто
  map.clear();
  auto begin_iterator = map.begin();
  assert(begin_iterator == map.end());
  std::cout << "All Iterator operations tests passed successfully!"
            << std::endl;

  // Тестируем устойчивость к коллизиям
  CustomUnorderedMap<std::string, float, SimpleStringHasher> map_new(8);
  map_new["abc"] = 3.4;
  map_new["bca"] = 2.5;
  map_new["cba"] = 1.5;
  map_new["xyz"] = 5.4;
  auto it = map_new.find("xyz");
  it->second = 5.3;
  // Проверка возможности изменения
  assert((*it).second == static_cast<float>(5.3));
  for (auto iterator = map_new.begin(); iterator != map_new.end(); ++iterator) {
    auto key_val = *iterator;
    // std::cout << key_val.first << ": " << key_val.second << std::endl;
  }
  map_new.erase("abc");
  map_new.erase("bca");
  map_new.erase("cba");

  // Тестируем перебор с учётом коллизий:
  for (auto iterator = map_new.begin(); iterator != map_new.end(); ++iterator) {
    auto key_val = *iterator;
  }
}

void testConstIterator() {
  // Тестируем константный итератор для хэш-таблицы
  CustomUnorderedMap<std::string, float> map(4);

  // Заполняем хэш-таблицу экспериментальными значениями
  map["apple"] = 0.25;
  map["watermelon"] = 1.24;
  map["tomato"] = 0.125;

  // Константный итератор на начало таблицы
  auto start_iter = map.cbegin();
  // Проверяем, что он не равен итератору на конец
  assert(start_iter != map.cend());
  auto start_iter_ = map.cbegin();
  // Проверяем оператор ==
  assert(start_iter_ == start_iter);
  // Проверяем оператор ++
  ++start_iter;
  // Не равен итератору на начало и на конец
  assert(start_iter != map.cbegin());
  assert(start_iter != map.cend());
  // Пробуем  сделать проход
  for (auto it = map.cbegin(); it != map.cend(); ++it) {
    auto key_val = (*it);
  }
  // Пробуем сделать изменение по итератору
  // start_iter->second = 0.5; Компилятор выдает ошибку error: cannot assign to
  // return value because function 'operator->' returns a const value

  std::cout << "All ConstIterator operations tests passed successfully!"
            << std::endl;
}

int main() {
  testHashTableOperations();
  testHashTableIterator();
  testConstIterator();
  return 0;
}