/*Реализуйте структуру данных типа “множество строк” на основе динамической
хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
строчных латинских букв. Хеш-функция строки должна быть реализована с помощью
вычисления значения многочлена методом Горнера. Начальный размер таблицы должен
быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна
поддерживать операции добавления строки в множество, удаления строки из
множества и проверки принадлежности данной строки множеству.

Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
Вариант 2. Для разрешения коллизий используйте двойное хеширование.*/

#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename T> struct HashTableNode {
  T data;
  HashTableNode(T value) : data(value) {}
  HashTableNode() : data("NIL") {}
};

class StringHasher {
public:
  StringHasher(size_t prime_number = 71) : prime_number(prime_number) {}

  size_t operator()(std::string str) {
    size_t hash = 0;
    for (int i = 0; i < str.size(); i++) {
      hash = hash * prime_number + str[i];
    }
    return hash;
  }

private:
  size_t prime_number;
};

template <typename T, typename Hasher> class HashTable {
public:
  HashTable(size_t initial_length) : table(initial_length), real_size(0) {}
  ~HashTable() {}
  bool Has(T key);
  bool Add(T key);
  bool Delete(T key);
  int get_real_size() // Functions for check & train
  {
    return real_size;
  }
  int get_buffer_size() { return table.size(); }

private:
  void grow(); // Re-hashing technique
  std::vector<HashTableNode<T>> table;
  Hasher hasher;
  int real_size;
};

template <typename T, typename Hasher> bool HashTable<T, Hasher>::Has(T key) {
  size_t hash_value = hasher(key) % table.size();
  for (int i = 0; i < table.size(); i++) {
    if (table[hash_value].data == key) {
      return true;
    } else {
      if (table[hash_value].data == "NIL") {
        return false;
      }
    }
    hash_value = (hash_value + i + 1) % table.size(); // Quadratic proba
  }
  return false;
}

template <typename T, typename Hasher> bool HashTable<T, Hasher>::Add(T key) {

  float alpha = float(real_size) / float(table.size());
  if (alpha >= 0.75) // Initial threshold
  {
    /*std:: cout << "grow call" << std::endl;*/ // Staff message
    grow();
  }
  size_t hash_value = hasher(key) % table.size();
  size_t first_del = table.size(); // Assume first DEL flag to be out of range
  for (int i = 0; i < table.size(); i++) {
    if (table[hash_value].data == key) {
      return false;
    } else {
      if (table[hash_value].data == "DEL" && first_del == table.size()) {
        first_del = hash_value; // Change flag
      }
      if (table[hash_value].data == "NIL") {
        if (first_del != table.size()) {
          hash_value = first_del;
        }
        table[hash_value].data = key;
        ++real_size;
        return true;
      }
    }
    hash_value = (hash_value + i + 1) % table.size();
  }

  if (first_del != table.size()) {
    hash_value = first_del;
  }
  table[hash_value].data = key;
  real_size++;
  return true;
}

template <typename T, typename Hasher>
bool HashTable<T, Hasher>::Delete(T key) {
  size_t hash_value = hasher(key) % table.size();
  for (int i = 0; i < table.size(); i++) {
    if (table[hash_value].data == key) {
      table[hash_value].data = "DEL";
      real_size--;
      return true;
    } else {
      if (table[hash_value].data == "NIL") {
        return false;
      }
    }
    hash_value = (hash_value + i + 1) % table.size();
  }
  return false;
}

template <typename T, typename Hasher> void HashTable<T, Hasher>::grow() {
  HashTable<T, Hasher> new_table(table.size() * 2);
  for (const auto &line : table) {
    if (line.data != "NIL" && line.data != "DEL") {
      new_table.Add(line.data);
    }
  }
  std::swap(new_table.table, table);
}

int main() {
  int initial_size = 8;
  HashTable<std::string, StringHasher> hash_table(initial_size);

  char op;
  std::string line;
  while (std::cin >> op >> line) {
    switch (op) {
    case '?': {
      std::cout << (hash_table.Has(line) ? "OK" : "FAIL") << std::endl;
      break;
    }
    case '+': {
      std::cout << (hash_table.Add(line) ? "OK" : "FAIL") << std::endl;
      break;
    }
    case '-': {
      std::cout << (hash_table.Delete(line) ? "OK" : "FAIL") << std::endl;
      break;
    }
    }
  }
  return 0;
}