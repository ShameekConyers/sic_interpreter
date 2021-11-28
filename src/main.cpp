#include "common.hpp"
#include "chunk.hpp"
#include "debug.hpp"
#include "vm.hpp"

int main(int argc, const char* argv [])
{
	VM vm;
	Chunk chunk;
	int idx_of_constant;
	idx_of_constant = chunk.add_constant(1.2);
	chunk.write(OpCode::OP_CONSTANT, 1);
	chunk.write(idx_of_constant, 1);

	idx_of_constant = chunk.add_constant(3.4);
	chunk.write(OpCode::OP_CONSTANT, 1);
	chunk.write(idx_of_constant, 1);

	chunk.write(OpCode::OP_ADD, 1);

	idx_of_constant = chunk.add_constant(5.6);
	chunk.write(OpCode::OP_CONSTANT, 1);
	chunk.write(idx_of_constant, 1);

	chunk.write(OpCode::OP_DIVIDE, 1);

	chunk.write(OpCode::OP_NEGATE, 1);
	chunk.write(OpCode::OP_RETURN, 1);

	disassemble_chunk(chunk, "test");
	vm.interpret(chunk);

	chunk.free();
	return 0;
}
