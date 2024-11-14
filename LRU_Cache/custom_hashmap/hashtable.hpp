#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <vector>

template <typename Key, typename Value, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class CustomUnorderedMap {
private:
  using KeyValuePair = std::pair<Key, Value>;

  struct Bucket {
    std::unique_ptr<KeyValuePair> key_val;
    bool is_deleted = false;
  };

  std::vector<std::list<Bucket>> table;
  size_t bucket_count;
  size_t current_size = 0;
  float max_load_factor_ = 0.7;
  Hash hash_function;
  KeyEqual key_equal;

  size_t hash_index(const Key &key) const {
    return hash_function(key) % bucket_count;
  }

  void rehash(bool reserve_flag, size_t input_size) {
    size_t new_bucket_count = bucket_count * 2;
    if (reserve_flag) {
      new_bucket_count = input_size;
    }
    std::vector<std::list<Bucket>> new_table(new_bucket_count);
    for (const auto &bucket_list : table) {
      for (const auto &bucket : bucket_list) {
        if (bucket.key_val && !bucket.is_deleted) {
          size_t index =
              hash_function(bucket.key_val->first) % new_bucket_count;
          new_table[index].emplace_back(
              Bucket{std::make_unique<KeyValuePair>(*bucket.key_val), false});
        }
      }
    }
    table = std::move(new_table);
    bucket_count = new_bucket_count;
  }

public:
  CustomUnorderedMap(size_t initial_bucket_count = 8)
      : bucket_count(initial_bucket_count) {
    table.resize(initial_bucket_count);
  }

  ~CustomUnorderedMap() = default;

  bool insert(const Key &key, const Value &value) {
    size_t index = hash_index(key);

    for (auto &bucket : table[index]) {
      if (bucket.key_val && key_equal(bucket.key_val->first, key)) {
        bucket.key_val->second = value;
        return true;
      }
    }

    if ((current_size + 1) > (bucket_count * max_load_factor_)) {
      rehash(false, bucket_count);
    }

    table[index].emplace_back(
        Bucket{std::make_unique<KeyValuePair>(key, value), false});
    current_size++;
    return true;
  }

  bool erase(const Key &key) {
    size_t index = hash_index(key);

    for (auto &bucket : table[index]) {
      if (bucket.key_val && !bucket.is_deleted &&
          key_equal(bucket.key_val->first, key)) {
        bucket.is_deleted = true;
        current_size--;
        return true;
      }
    }
    return false;
  }

  size_t size() const { return current_size; }

  bool empty() const { return current_size == 0; }

  void clear() {
    for (auto &bucket_list : table) {
      bucket_list.clear();
    }
    current_size = 0;
  }

  void reserve(size_t new_bucket_count) {
    if (new_bucket_count > bucket_count) {
      bucket_count = new_bucket_count;
      rehash(true, new_bucket_count);
    }
  }

  float load_factor() const {
    return static_cast<float>(current_size) / bucket_count;
  }

  void max_load_factor(float new_max_load_factor) {
    max_load_factor_ = new_max_load_factor;
  }

  Value &operator[](const Key &key) {
    size_t index = hash_index(key);

    for (auto &bucket : table[index]) {
      if (bucket.key_val && key_equal(bucket.key_val->first, key)) {
        return bucket.key_val->second;
      }
    }

    if ((current_size + 1) > (bucket_count * max_load_factor_)) {
      rehash(false, bucket_count);
      index = hash_index(key);
    }

    auto new_kv = std::make_unique<KeyValuePair>(key, Value());
    table[index].emplace_back(Bucket{std::move(new_kv), false});
    current_size++;
    return table[index].back().key_val->second;
  }

  size_t count(const Key &key) const {
    size_t index = hash_index(key);

    for (const auto &bucket : table[index]) {
      if (bucket.key_val && key_equal(bucket.key_val->first, key)) {
        return 1;
      }
    }
    return 0;
  }

  class Iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = KeyValuePair;
    using pointer = value_type *;
    using reference = value_type &;

    Iterator(CustomUnorderedMap *map, size_t bucket_index,
             typename std::list<Bucket>::iterator bucket_it)
        : map(map), bucket_index(bucket_index), bucket_it(bucket_it) {
      find_valid();
    }

    Iterator &operator++() {
      ++bucket_it;
      find_valid();
      return *this;
    }

    bool operator!=(const Iterator &other) const {
      return bucket_it != other.bucket_it || bucket_index != other.bucket_index;
    }

    bool operator==(const Iterator &other) const {
      return bucket_it == other.bucket_it && bucket_index == other.bucket_index;
    }

    reference operator*() const { return *bucket_it->key_val; }
    pointer operator->() const { return bucket_it->key_val.get(); }

  private:
    CustomUnorderedMap *map;
    size_t bucket_index;
    typename std::list<Bucket>::iterator bucket_it;

    void find_valid() {
      while (bucket_index < map->bucket_count) {
        while (bucket_it != map->table[bucket_index].end()) {
          if (bucket_it->key_val && !bucket_it->is_deleted) {
            return;
          }
          ++bucket_it;
        }
        ++bucket_index;
        if (bucket_index < map->bucket_count) {
          bucket_it = map->table[bucket_index].begin();
        }
      }
      bucket_it = map->table[bucket_index]
                      .end(); // Устанавливаем на end() если ничего не найдено
    }
  };

  class ConstIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const KeyValuePair;
    using pointer = const value_type *;
    using reference = const value_type &;

    ConstIterator(const CustomUnorderedMap *map, size_t bucket_index,
                  typename std::list<Bucket>::const_iterator bucket_it)
        : map(map), bucket_index(bucket_index), bucket_it(bucket_it) {
      find_valid();
    }

    ConstIterator &operator++() {
      ++bucket_it;
      find_valid();
      return *this;
    }

    bool operator!=(const ConstIterator &other) const {
      return bucket_it != other.bucket_it || bucket_index != other.bucket_index;
    }

    bool operator==(const ConstIterator &other) const {
      return bucket_it == other.bucket_it && bucket_index == other.bucket_index;
    }

    reference operator*() const { return *bucket_it->key_val; }
    pointer operator->() const { return bucket_it->key_val.get(); }

  private:
    const CustomUnorderedMap *map;
    size_t bucket_index;
    typename std::list<Bucket>::const_iterator bucket_it;

    void find_valid() {
      while (bucket_index < map->bucket_count) {
        while (bucket_it != map->table[bucket_index].end()) {
          if (bucket_it->key_val && !bucket_it->is_deleted) {
            return;
          }
          ++bucket_it;
        }
        ++bucket_index;
        if (bucket_index < map->bucket_count) {
          bucket_it = map->table[bucket_index].begin();
        }
      }
      bucket_it = map->table[bucket_index].end();
    }
  };

  ConstIterator cbegin() const {
    for (size_t i = 0; i < bucket_count; ++i) {
      if (!table[i].empty()) {
        return ConstIterator(this, i, table[i].begin());
      }
    }
    return cend();
  }

  ConstIterator cend() const {
    return ConstIterator(this, bucket_count,
                         typename std::list<Bucket>::const_iterator());
  }

  Iterator begin() {
    for (size_t i = 0; i < bucket_count; ++i) {
      if (!table[i].empty()) {
        return Iterator(this, i, table[i].begin());
      }
    }
    return end();
  }

  Iterator end() {
    return Iterator(this, bucket_count, typename std::list<Bucket>::iterator());
  }

  Iterator find(const Key &key) {
    size_t index = hash_index(key);

    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
      if (it->key_val && !it->is_deleted &&
          key_equal(it->key_val->first, key)) {
        return Iterator(this, index, it);
      }
    }
    return end(); // Если элемент не найден
  }

  ConstIterator find(const Key &key) const {
    size_t index = hash_index(key);

    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
      if (it->key_val && !it->is_deleted &&
          key_equal(it->key_val->first, key)) {
        return ConstIterator(this, index, it);
      }
    }
    return end(); // Если элемент не найден
  }
};

#endif // HASHTABLE_HPP