#pragma once

#include "chunk.hpp"

void disassemble_chunk(const Chunk& chunk, const char* name);
int disassemble_instruction(const Chunk& chunk, int offset);
int static simple_instruction(const char* name, int offset);
int static constant_instruction(const char* name, int offset);

// END DECLARATIONS
#include <iostream>

void disassemble_chunk(const Chunk& chunk, const char* name)
{
	std::cout << "== " << name << "==\n";

	for (int offset = 0; offset < chunk.m_count;) {
		offset = disassemble_instruction(chunk, offset);
	}
}

int static simple_instruction(const char* name, int offset)
{
	std::cout << name << "\n";
	return offset + 1;
}

int static constant_instruction(
	const char* name,
	const Chunk& chunk,
	int offset)
{
	uint8_t idx = chunk.m_data[offset + 1];
	std::cout << name << ":\t";
	std::cout << +idx << ", " << chunk.m_constants.m_data[idx];
	std::cout << "\n";
	return offset + 2;
}



int disassemble_instruction(const Chunk& chunk, int offset)
{
	std::cout << offset << ":\t ";

	uint8_t instruction = chunk.m_data[offset];
	switch (instruction) {
		case OpCode::OP_RETURN:
			return simple_instruction("OP_RETURN", offset);
		case OpCode::OP_CONSTANT:
			return constant_instruction("OP_CONSTANT", chunk, offset);
		default:
			std::cout << "Unknown opcode " << instruction << "\n";
			return offset + 1;
	}
}
