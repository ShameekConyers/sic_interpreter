#pragma once
#include "common.hpp"
#include "value.hpp"
#include "memory.hpp"

inline const double OBJECTMAP_MAX_LOAD = 0.5;

struct Entry {
  String* m_key = nullptr;
  Value m_value;
  Entry* m_next; // optional for chaining
};

struct ObjectMap {
  size_t m_count;
  size_t m_capacity;
  Entry* m_entries;

  //

  ObjectMap()
  {
    init_OM(this);
  }

  ~ObjectMap()
  {
    free_OM(this);
  }

  Value* get(String* key)
  {
    if (m_entries == nullptr) return nullptr;
    Entry* entry = find_entry(key);
    if (entry->m_key == nullptr) return nullptr;
    return &entry->m_value;
  }

  bool insert(String* key, Value value)
  {
    check_map_load();
    Entry* entry = find_entry(key);
    bool is_new_key = (entry->m_key == nullptr);
    if (is_new_key && entry->m_value.is_type<nullptr_t>()) {
      m_count++;
    }

    entry->m_key = key;
    entry->m_value = value;


    return is_new_key;
  }

  bool remove(String* key)
  {
    if (m_entries == nullptr) return false;

    Entry* entry = find_entry(key);
    if (entry->m_key == nullptr) return false;

    entry->m_key = nullptr;
    entry->m_value = Value::make<bool>(true);
    return true;
  }

  void check_map_load()
  {
    if (m_count + 1 > m_capacity * OBJECTMAP_MAX_LOAD) {
      adjust_capacity();
    }
  }

  void adjust_capacity()
  {
    size_t next_capacity = grow_capacity(m_capacity);
    Entry* next_entries = allocate<Entry>(next_capacity);

    m_count = 0; // to redo counter for clearing tombstones

    for (size_t i = 0; i < next_capacity; i++) {
      next_entries[i].m_key = nullptr;
      next_entries[i].m_value = Value::make<nullptr_t>(nullptr);
    }

    for (size_t i = 0; i < m_capacity; i++) {
      Entry* entry = &m_entries[i];

      Entry* dest = ObjectMap::find_entry(next_entries, next_capacity, entry->m_key);
      dest->m_key = next_entries->m_key;
      dest->m_value = next_entries->m_value;
      m_count++;
    }

    free_array(m_entries, m_capacity);
    m_entries = next_entries;
    m_capacity = next_capacity;

  }

  Entry* find_entry(String* key)
  {
    return ObjectMap::find_entry(m_entries, m_capacity, key);
  }

  static Entry* find_entry(Entry* entries, size_t capacity, String* key)
  {
    uint64_t index = key->m_hash % capacity;
    Entry* tombstone_memo = nullptr;
    while (true) {
      Entry* entry = &entries[index];

      if (entry->m_key == nullptr) {
        if (entry->m_value.is_type<nullptr_t>()) {
          // empty
          return tombstone_memo == nullptr ? entry : tombstone_memo;
        }
        else {
          // tombstone
          if (tombstone_memo == nullptr) tombstone_memo = entry;
        }
      }
      else if (entry->m_key == key) {
        return entry;
      }

      index = (index + 1) % capacity;
    }

  }
  static void copy_object_map(ObjectMap* from, ObjectMap* to)
  {
    for (size_t i = 0; i < from->m_capacity; i++) {
      Entry* entry = &from->m_entries[i];
      if (entry->m_key != nullptr) {
        to->insert(entry->m_key, entry->m_value);
      }
    }
  }

  static void init_OM(ObjectMap* object_map)
  {
    object_map->m_count = 0;
    object_map->m_capacity = 0;
    object_map->m_entries = nullptr;
    object_map->adjust_capacity();
  }

  static void grow_OM(ObjectMap* object_map)
  {

  }

  static void free_OM(ObjectMap* object_map)
  {
    free_array<Entry>(object_map->m_entries, object_map->m_capacity);
    init_OM(object_map);
  }
};

ObjectMap om{};
