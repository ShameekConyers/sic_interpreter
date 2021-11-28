#pragma once



#include <iostream>
#include <functional>

#include "chunk.hpp"
#include "value.hpp"


namespace InterpretResult
{
enum Enum {
	INTERPRET_OK,
	INTERPRET_CONSTANT
};
}

template<typename T, size_t STACK_SIZE = 256>
struct StaticStack {

	StaticStack()
	{
		m_data_top = m_data;
	}

	T top()
	{
		return *m_data_top;
	}

	void push(T item)
	{
		*m_data_top = item;
		m_data_top++;
	}

	T pop()
	{
		m_data_top--;
		return *m_data_top;
	}

	void clear()
	{
		m_data_top = m_data;
	}

	void print()
	{
		for (Value* slot = m_data; slot < m_data_top; slot++) {
			std::cout << "[ " << *slot << " ] ";
		}
		std::cout << "\n";
	}

	T m_data[STACK_SIZE];
	T* m_data_top;
};

struct VM {

	InterpretResult::Enum interpret(Chunk& chunk)
	{
		m_chunk = &chunk;
		m_instruction_ptr = m_chunk->m_data;
		return run();
	}

	void init()
	{
		reset_vm_stack();
	}

	InterpretResult::Enum run()
	{
		for (;;) {

#ifdef DEBUG_TRACE_EXECUTION
			m_vm_stack.print();
			disassemble_instruction(*m_chunk, (int)(m_instruction_ptr - m_chunk->m_data));
#endif

			uint8_t instruction;
			switch (instruction = adv_instruction()) {
				case OpCode::OP_RETURN:
				{
					return InterpretResult::INTERPRET_OK;
					m_vm_stack.pop();
					break;
				}
				case OpCode::OP_CONSTANT:
				{
					Value constant = read_constant(adv_instruction());
					m_vm_stack.push(constant);
					break;
				}
				case OpCode::OP_NEGATE:
				{
					m_vm_stack.push(-m_vm_stack.pop());
					break;
				}
				case OpCode::OP_ADD:
				{
					binary_op(std::plus());
					break;
				}
				case OpCode::OP_SUBTRACT:
				{
					binary_op(std::minus());
					break;
				}
				case OpCode::OP_DIVIDE:
				{
					binary_op(std::divides());
					break;
				}
				case OpCode::OP_MULTIPLY:
				{
					binary_op(std::multiplies());
					break;
				}
			}
		}


	}

	uint8_t inline adv_instruction()
	{
		return *m_instruction_ptr++;
	}

	void free()
	{

	}

	Value read_constant(uint8_t idx)
	{
		return m_chunk->m_constants.m_data[idx];
	}


	void reset_vm_stack()
	{
		m_vm_stack.clear();
	}

	template<typename Fn>
	void binary_op(const Fn& func)
	{
		Value b = m_vm_stack.pop();
		Value a = m_vm_stack.pop();
		m_vm_stack.push(func(a, b));
	}

	Chunk* m_chunk;
	uint8_t* m_instruction_ptr;
	StaticStack<Value> m_vm_stack;
};
