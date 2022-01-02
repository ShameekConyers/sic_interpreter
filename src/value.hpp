#pragma once
#include "common.hpp"
#include "base_array.hpp"
#include "memory.hpp"
#include "exception.hpp"
#include <iostream>

namespace ValueType
{
enum Any {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUM,
  VAL_OBJ // Lives on heap
};
}
namespace ObjType
{
enum Any {
  OBJ_STR
};
}



using string = std::string;
struct Obj;
struct String;

struct Obj {
  string m_name;
  ObjType::Any m_type;


  static Obj* allocate_object(ObjType::Any type);

  template<typename T>
  static Obj* allocate_object();

  string to_str_literal();

#ifndef __INTELLISENSE__
  template<>
  Obj* allocate_object<String>();
#endif
private:
};

struct String : public Obj {
  int m_length;
  char* m_chars;

  String(const char* chars, int length)
  {

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
    string->m_length = length;
    string->m_chars = heap_chars;
    return string;
  }

  string to_str_literal()
  {
    return m_chars;
  }
};


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
  return obj;
}

string Obj::to_str_literal()
{
  switch (m_type) {
    case ObjType::OBJ_STR:
      return reinterpret_cast<String*>(this)->to_str_literal();
      break;
    default:
      break;
  }
}

using ObjPtr = Obj*;
using Number = double;
struct Value {
  ValueType::Any m_type;
  union {
    bool boolean;
    double number;
    nullptr_t nil;
    ObjPtr obj;
  } m_data;

  void set_bool(bool value)
  {
    m_data.boolean = value;
  }

  void set_number(double value)
  {
    m_data.number = value;
  }

  void set_nil(nullptr_t value)
  {
    m_data.nil = value;
  }

  void set_obj(ObjPtr val)
  {
    m_data.obj = val;
  }

  bool as_bool()
  {
    return m_data.boolean;
  }

  double as_number()
  {
    return m_data.number;
  }

  void as_nil()
  {

  }

  ObjPtr as_obj()
  {
    return m_data.obj;
  }

  String* as_string()
  {
    return reinterpret_cast<String*>(as_obj());
  }

  char* as_cstring()
  {
    return as_string()->m_chars;
  }

  ObjType::Any obj_type()
  {
    return m_data.obj->m_type;
  }


  template<typename T>
  T get()
  {

  }

  // Value static make(ValueType::Any val)
  // {
  //   switch (val) {
  //     case /* constant-expression */:
  //       /* code */
  //       break;

  //     default:
  //       break;
  //   }

  //   return
  // }

  template<typename T>
  Value static make(T val)
  {

  }

  bool is_type(ValueType::Any val)
  {
    return m_type == val;
  }

  template<typename T>
  bool is_type()
  {

  };

  bool is_obj_type(ObjType::Any type)
  {
    return is_type<ObjPtr>() && as_obj()->m_type == type;
  };

  std::string to_str()
  {
    switch (m_type) {
      case ValueType::VAL_BOOL:
        return m_data.boolean ? "true" : "false";
        break;
      case ValueType::VAL_NUM:
        return std::to_string(m_data.number);
        break;
      case ValueType::VAL_NIL:
        return "nil";
        break;
      case ValueType::VAL_OBJ:
        return m_data.obj->to_str_literal();
        break;
      default:
        return "ERROR";
        break;
    }
  }

#ifndef __INTELLISENSE__
  template<>
  Number get<Number>()
  {
    if (m_type != ValueType::VAL_NUM) {

    }
    return m_data.number;
  }

  template<>
  bool get<bool>()
  {
    if (m_type != ValueType::VAL_BOOL) {

    }
    return m_data.boolean;
  }

  template<>
  nullptr_t get<nullptr_t>()
  {
    if (m_type != ValueType::VAL_NIL) {

    }
    return m_data.nil;
  }

  template<>
  ObjPtr get<ObjPtr>()
  {
    if (m_type != ValueType::VAL_OBJ) {

    }
    return m_data.obj;
  }

  //

  template<>
  Value make<Number>(Number val)
  {
    Value v;
    v.m_type = ValueType::VAL_NUM;
    v.set_number(val);
    return v;
  }

  template<>
  Value make<bool>(bool val)
  {
    Value v;
    v.m_type = ValueType::VAL_BOOL;
    v.set_bool(val);
    return v;
  }

  template<>
  Value make<nullptr_t>(nullptr_t val)
  {
    Value v;
    v.m_type = ValueType::VAL_NIL;
    v.set_nil(val);
    return v;
  }

  template<>
  Value make<ObjPtr>(ObjPtr val)
  {
    Value v;
    v.m_type = ValueType::VAL_OBJ;
    v.set_obj(val);
    return v;
  }

  //

  template<>
  bool is_type<double>()
  {
    if (m_type == ValueType::VAL_NUM) {
      return true;
    }
    return false;
  }

  template<>
  bool is_type<bool>()
  {
    if (m_type == ValueType::VAL_BOOL) {
      return true;
    }
    return false;
  }

  template<>
  bool is_type<nullptr_t>()
  {
    if (m_type == ValueType::VAL_NIL) {
      return true;
    }
    return false;
  }

  template<>
  bool is_type<ObjPtr>()
  {
    if (m_type == ValueType::VAL_OBJ) {
      return true;
    }
    return false;
  }
#endif
  friend std::ostream& operator<<(std::ostream& output_stream, Value& value)
  {
    output_stream << value.to_str();
    return output_stream;
  }

  friend bool is_value_false(Value value)
  {
    return value.is_type<nullptr_t>() || (value.is_type<bool>() && !value.as_bool());
  }

  friend bool is_values_equal(Value a, Value b)
  {
    if (a.m_type != b.m_type) return false;

    switch (a.m_type) {
      case ValueType::VAL_BOOL: return a.get<bool>() == b.get<bool>();
      case ValueType::VAL_NIL: return true;
      case ValueType::VAL_NUM: return a.get<Number>() == b.get<Number>();
      default: return false; // unreachable
    }
  }
    };


using ValueArray = BaseArray<Value>;
