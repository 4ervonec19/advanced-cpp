#ifndef LRU_HPP
#define LRU_HPP

#include "hashtable.hpp"
#include <iostream>
#include <list>
#include <vector>

template <typename keyT, typename valueT> class LRU_Cache {
public:
  LRU_Cache(size_t N, size_t Bytes)
      : max_size(N), max_bytes(Bytes), current_bytes_size(0) {}

  size_t size() const { return cache_list.size(); }

  size_t size_bytes() const { return current_bytes_size; }

  void clear() {
    cache_list.clear();
    cache_unordered_map.clear();
    current_bytes_size = 0;
  }

  bool add_note(const keyT &input_key, const valueT &input_value) {
    size_t input_size = get_note_bytes(input_key, input_value);

    if (input_size > max_bytes || max_size == 0) {
      return false;
    }

    auto it = cache_unordered_map.find(input_key);
    if (it == cache_unordered_map.end()) {
      while ((current_bytes_size + input_size > max_bytes)) {
        delete_last();
      }
    } else {
      current_bytes_size -=
          get_note_bytes((*it).second->first, (*it).second->second);
      cache_unordered_map.erase((*it).second->first);
      cache_list.erase((*it).second);
    }

    cache_list.emplace_front(input_key, input_value);
    cache_unordered_map[input_key] = cache_list.begin();
    current_bytes_size += input_size;

    if (cache_list.size() > max_size) {
      delete_last();
    }

    return true;
  }

  bool get(const keyT &query_key) {
    auto it = cache_unordered_map.find(query_key);
    if (it ==
        cache_unordered_map.end()) // Если ключ не найден, выводим "!NOEMBED!"
    {
      std::cout << "!NOEMBED!" << std::endl;
      return false;
    }

    // Перемещаем найденный элемент в начало списка
    cache_list.splice(cache_list.begin(), cache_list, (*it).second);

    // Вывод значений
    for (const auto &element : (*it).second->second) {
      std::cout << element << " ";
    }
    std::cout << std::endl;
    return true;
  }

  void print_cache() const {
    std::cout << "Cache content:" << std::endl;
    for (const auto &item : cache_list) {
      std::cout << "Key: " << item.first << ", Values: ";
      for (const auto &value : item.second) {
        std::cout << value << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "Current size: " << size() << ", Bytes used: " << size_bytes()
              << std::endl;
  }

private:
  // Подсчёт байтов строки в UTF-8
  size_t get_utf8_byte_size(const std::string &str) {
    size_t byte_size = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      unsigned char c = static_cast<unsigned char>(str[i]);
      if ((c & 0x80) == 0) { // 1 байт (ASCII)
        byte_size += 1;
      } else if ((c & 0xE0) == 0xC0) { // 2 байта
        byte_size += 2;
      } else if ((c & 0xF0) == 0xE0) { // 3 байта
        byte_size += 3;
      } else if ((c & 0xF8) == 0xF0) { // 4 байта
        byte_size += 4;
      }
    }
    return byte_size;
  }

  size_t get_cp1251_byte_size(const std::string &str) {
    return str.size(); // В CP1251 каждый символ занимает 1 байт
  }

  bool is_utf8(const std::string &str) {
    for (unsigned char c : str) {
      if (c & 0x80) { // Если есть байт с установленным 8-м битом, это UTF-8
        return true;
      }
    }
    return false;
  }

  size_t get_string_byte_size(const std::string &str) {
    if (is_utf8(str)) {
      return get_utf8_byte_size(str);
    }
    return str.size(); // Для CP1251 каждый символ занимает 1 байт
  }

  size_t get_note_bytes(const keyT &input_key, const valueT &input_value) {
    return get_string_byte_size(input_key) +
           input_value.size() * sizeof(input_value[0]);
  }

  void delete_last() {
    if (cache_list.empty()) {
      return;
    }

    auto last_note = cache_list.back();
    current_bytes_size -= get_note_bytes(last_note.first, last_note.second);
    cache_unordered_map.erase(last_note.first);
    cache_list.pop_back();
  }

  size_t max_size;
  size_t max_bytes;
  size_t current_bytes_size;

  std::list<std::pair<keyT, valueT>> cache_list;
  CustomUnorderedMap<keyT,
                     typename std::list<std::pair<keyT, valueT>>::iterator>
      cache_unordered_map;
};

#endif // LRU_HPP
