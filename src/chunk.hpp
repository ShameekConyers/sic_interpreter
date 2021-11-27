#pragma once

#include "common.hpp"
#include "memory.hpp"
#include "base_array.hpp"
#include "value.hpp"


namespace OpCode
{
enum {
	OP_CONSTANT, // 2 Bytes
	OP_RETURN,	// 1 Byte
};

}

// wrapper around an array of bytes

// using Chunk = BaseArray<uint8_t>;

struct Chunk : public BaseArray<uint8_t> {
	Chunk()
		: BaseArray<uint8_t>()
		, m_constants{}
	{

	}

	// returns the index of the value added
	int add_constant(Value value)
	{
		m_constants.write(value);
		return m_constants.m_count - 1;
	}

	ValueArray m_constants;
};
