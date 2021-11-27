#include "common.hpp"
#include "chunk.hpp"
#include "debug.hpp"

int main(int argc, const char* argv [])
{
	Chunk chunk;
	int idx_of_constant = chunk.add_constant(1.2);
	chunk.write(OpCode::OP_CONSTANT, 1);
	chunk.write(idx_of_constant, 1);

	chunk.write(OpCode::OP_RETURN, 1);
	disassemble_chunk(chunk, "test");

	chunk.free();
	return 0;
}
