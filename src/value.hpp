#pragma once
#include "common.hpp"
#include "base_array.hpp"

#include <iostream>

namespace ValueType
{
enum Any {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER
};
}

using Number = double;
struct Value {
  ValueType::Any m_type;
  union {
    bool boolean;
    double number;
    nullptr_t nil;
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

  template<typename T>
  T get()
  {

  }

  template<typename T>
  Value static make(T val)
  {

  }

  template<typename T>
  bool is_type()
  {

  };

  std::string to_str()
  {
    switch (m_type) {
      case ValueType::VAL_BOOL:
        return m_data.boolean ? "true" : "false";
        break;
      case ValueType::VAL_NUMBER:
        return std::to_string(m_data.number);
        break;
      case ValueType::VAL_NIL:
        return "nil";
        break;
      default:
        return "ERROR";
        break;
    }
  }

#ifndef __INTELLISENSE__
  template<>
  bool get<bool>()
  {
    if (m_type != ValueType::VAL_BOOL) {

    }
    return m_data.boolean;
  }

  template<>
  Number get<Number>()
  {
    if (m_type != ValueType::VAL_NUMBER) {

    }
    return m_data.number;
  }

  template<>
  Value make<Number>(Number val)
  {
    Value v;
    v.m_type = ValueType::VAL_NUMBER;
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
  bool is_type<double>()
  {
    return true;
  }

  template<>
  bool is_type<bool>()
  {
    return true;
  }
#endif
  friend std::ostream& operator<<(std::ostream& output_stream, Value& value)
  {
    output_stream << value.to_str();
    return output_stream;
  }
};


using ValueArray = BaseArray<Value>;
