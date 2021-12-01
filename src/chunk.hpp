#pragma once

#include "common.hpp"
#include "memory.hpp"
#include "base_array.hpp"
#include "value.hpp"


namespace OpCode
{
enum {
  OP_CONSTANT, // 2 Bytes
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,	// 1 Byte
  OP_RETURN,	// 1 Byte
};

}

// wrapper around an array of bytes

// using Chunk = BaseArray<uint8_t>;
using LineArray = BaseArray<uint8_t>;

struct Chunk : public BaseArray<uint8_t> {
  Chunk()
    : BaseArray<uint8_t>()
    , m_constants{}
    , m_lines{}
  {

  }

  void write(uint8_t byte, int line)
  {
    write(byte);
    m_lines.write(line);
  }

  // returns the index of the value added
  int add_constant(Value value)
  {
    m_constants.write(value);
    return m_constants.m_count - 1;
  }

  ValueArray m_constants;
  LineArray m_lines;

private:
  using BaseArray<uint8_t>::write;
};
