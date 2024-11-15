#ifndef LRU_HPP
#define LRU_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
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
      if (it != cache_unordered_map.end()) {
        std::cout << it->second->first << std::endl;
        current_bytes_size -=
            get_note_bytes(it->second->first, it->second->second);
        cache_list.erase(it->second);
        cache_unordered_map.erase(it->first);
      }
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
    if (it == cache_unordered_map.end()) {
      std::cout << "!NOEMBED!" << std::endl;
      return;
    }

    cache_list.splice(cache_list.begin(), cache_list, it->second);
    for (const auto &element : it->second->second) {
      std::cout << element << " ";
    }
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
  size_t get_note_bytes(const keyT &input_key, const valueT &input_value) {
    return input_key.size() * sizeof(input_key[0]) +
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
  std::unordered_map<keyT,
                     typename std::list<std::pair<keyT, valueT>>::iterator>
      cache_unordered_map;
};

#endif // LRU_HPP
