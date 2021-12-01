#pragma once
#include "memory.hpp"

// Wrapper around an array
template<typename T>
struct BaseArray {
  BaseArray()
  {
    init();
  }

  ~BaseArray()
  {
    free();
  }

  void init()
  {
    m_count = 0;
    m_capacity = 0;
    m_data = nullptr;
  }

  void write(T input)
  {
    if (m_capacity < m_count + 1) {
      ssize_t old_capacity = m_capacity;
      m_capacity = grow_capacity(m_capacity);
      m_data = grow_array(m_data, old_capacity, m_capacity);
    }

    m_data[m_count] = input;
    m_count++;
  }


  void free()
  {
    free_array(m_data, m_capacity);
    init();
  }

  ssize_t m_count;
  ssize_t m_capacity;
  T* m_data;
};
