#pragma once
#include "common.hpp"
#include "base_array.hpp"
#include "memory.hpp"
#include "exception.hpp"
#include <iostream>

namespace ObjType
{
enum Any {
  OBJ_STR
};
}

using string = std::string;
struct String;
struct Obj;
void update_vm_obj_list(Obj* obj);

uint64_t hash_string(const char* chars, int length)
{
  //  FNV-1a 64-bit
  uint64_t hash = 14695981039346656037u;
  for (int i = 0; i < length; i++) {
    hash ^= (uint8_t)chars[i];
    hash *= 1099511628211;
  }
  return hash;
}

struct Obj {
  string m_name;
  ObjType::Any m_type;
  struct Obj* m_next;
  uint64_t m_hash;

  static Obj* allocate_object(ObjType::Any type);

  template<typename T>
  static Obj* allocate_object();

  template<typename T>
  static Obj* take_object();

  string print_object();

  Obj& operator+(Obj& other);
  Obj& operator-(Obj& other);
  Obj& operator*(Obj& other);
  Obj& operator/(Obj& other);
#ifndef __INTELLISENSE__
  template<>
  Obj* allocate_object<String>();

  template<>
  Obj* take_object<String>();
#endif
private:
};

struct String : public Obj {
  int m_length;
  char* m_chars;

  String(const char* chars, int length)
  {

  }

  bool operator==(String& other)
  {
    return (m_length == other.m_length)
      && (memcmp(m_chars, other.m_chars, m_length) == 0);
  }

  static String* copy_str(const char* chars, int length)
  {
    char* heap_chars = allocate<char>(length + 1);
    memcpy(heap_chars, chars, length);
    heap_chars[length] = '\0';
    return allocate_string(heap_chars, length);
  }

  static String* allocate_string(char* heap_chars, int length)
  {
    String* string = reinterpret_cast<String*>(Obj::allocate_object<String>());
    string->m_hash = hash_string(heap_chars, length);
    string->m_length = length;
    string->m_chars = heap_chars;
    return string;
  }

  static String* take_string(char* heap_chars, int length)
  {
    String* string = reinterpret_cast<String*>(Obj::allocate_object<String>());
    string->m_hash = hash_string(heap_chars, length);
    string->m_length = length;
    string->m_chars = heap_chars;
    return string;
  }

  static String* concatenate(String* lhs, String* rhs)
  {
    int length = lhs->m_length + rhs->m_length;
    char* heap_chars = allocate<char>(length + 1);
    memcpy(heap_chars, lhs->m_chars, rhs->m_length);
    memcpy(heap_chars + lhs->m_length, rhs->m_chars, rhs->m_length);
    heap_chars[length] = '\0';
    return String::take_string(heap_chars, length);

  }

  string to_str_literal()
  {
    return m_chars;
  }

  String& operator+(String& other)
  {
    return *String::concatenate(this, &other);
  }

};

template<>
void free_type<String>(String* ptr)
{
  reallocate(ptr, sizeof(String), 0);
}



Obj* Obj::allocate_object(ObjType::Any type)
{
  switch (type) {
    case ObjType::OBJ_STR:
    {
      Obj* obj = allocate<Obj>(sizeof(String));
      obj->m_name = "String";
      obj->m_type = type;
      return obj;
      break;
    }
    default:
      runtime_exception();
      break;
  }
}

template<>
Obj* Obj::allocate_object<String>()
{
  Obj* obj = allocate<Obj>(sizeof(String));
  obj->m_type = ObjType::OBJ_STR;
  update_vm_obj_list(obj);
  return obj;
}

string Obj::print_object()
{
  switch (m_type) {
    case ObjType::OBJ_STR:
      return reinterpret_cast<String*>(this)->to_str_literal();
      break;
    default:
      break;
  }
}

Obj& Obj::operator+(Obj& other)
{
  switch (m_type) {
    case ObjType::OBJ_STR:
      return *(reinterpret_cast<String*>(this)) + (reinterpret_cast<String&>(other));
    default:
      break;
  }
}

Obj& Obj::operator-(Obj& other)
{
  switch (m_type) {
    case ObjType::OBJ_STR:
      return *(reinterpret_cast<String*>(this)) + (reinterpret_cast<String&>(other));
    default:
      break;
  }
}

Obj& Obj::operator*(Obj& other)
{
  switch (m_type) {
    case ObjType::OBJ_STR:
      return *(reinterpret_cast<String*>(this)) + (reinterpret_cast<String&>(other));
    default:
      break;
  }
}

Obj& Obj::operator/(Obj& other)
{
  switch (m_type) {
    case ObjType::OBJ_STR:
      return *(reinterpret_cast<String*>(this)) + (reinterpret_cast<String&>(other));
    default:
      break;
  }
}


void free_object(Obj* obj)
{
  switch (obj->m_type) {
    case ObjType::OBJ_STR:
    {
      String* str = reinterpret_cast<String*>(obj);
      free_array<char>(str->m_chars, str->m_length + 1);
      free_type<String>(str);
      break;
    }
    default:
      break;
  }
}

using ObjPtr = Obj*;

bool is_objects_equal(ObjPtr a, ObjPtr b)
{
  if (a->m_type != b->m_type) return false;
  switch (a->m_type) {
    case ObjType::OBJ_STR:
      return *reinterpret_cast<String*>(a) == *reinterpret_cast<String*>(b);
    default:
      break;
  }
}
