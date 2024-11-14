#ifndef LRU_HPP
#define LRU_HPP

#include "hashtable.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
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
      cache_list.erase((*it).second);
      cache_unordered_map.erase((*it).second->first);
    }

    cache_list.emplace_front(input_key, input_value);
    cache_unordered_map[input_key] = cache_list.begin();
    current_bytes_size += input_size;

    if (cache_list.size() > max_size) {
      delete_last();
    }

    return true;
  }

  void get(const keyT &query_key) {
    auto it = cache_unordered_map.find(query_key);
    if (it ==
        cache_unordered_map.end()) // Если ключ не найден, выводим "!NOEMBED!"
    {
      std::cout << "!NOEMBED!" << std::endl;
      return;
    }

    // Перемещаем найденный элемент в начало списка
    cache_list.splice(cache_list.begin(), cache_list, (*it).second);

    // Вывод значений
    for (const auto &element : (*it).second->second) {
      std::cout << element << " ";
    }
    // std::cout << get_note_bytes((*it).second->first, (*it).second->second);
    std::cout << std::endl;
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
  size_t get_utf8_byte_size(const std::string &str) {
    size_t byte_size = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      unsigned char c = static_cast<unsigned char>(str[i]);
      if ((c & 0x80) == 0) { // 0xxxxxxx
        byte_size += 1;
      } else if ((c & 0xE0) == 0xC0) { // 110xxxxx
        byte_size += 2;
      } else if ((c & 0xF0) == 0xE0) { // 1110xxxx
        byte_size += 3;
      } else if ((c & 0xF8) == 0xF0) { // 11110xxx
        byte_size += 4;
      }
    }
    return byte_size;
  }

  size_t get_cp1251_byte_size(const std::string &str) {
    return str.size(); // В CP1251 каждый символ занимает 1 байт
  }

  bool is_utf8(const std::string &str) {
    for (size_t i = 0; i < str.size(); ++i) {
      unsigned char c = static_cast<unsigned char>(str[i]);
      if (c >= 0x80) {
        return true; // Наличие символа >= 0x80 указывает на UTF-8
      }
    }
    return false; // Все символы <= 0x7F, это CP1251
  }

  size_t get_note_bytes(const keyT &input_key, const valueT &input_value) {
    size_t byte_size;
    if (is_utf8(input_key)) {
      byte_size = get_utf8_byte_size(input_key);
    } else {
      byte_size = get_cp1251_byte_size(input_key);
    }
    return byte_size + input_value.size() * sizeof(input_value[0]);
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
