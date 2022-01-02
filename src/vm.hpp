#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <cstdarg>
#include <fmt/core.h>

#include "chunk.hpp"
#include "value.hpp"
#include "compiler.hpp"


namespace InterpretResult
{
enum Any {
  OK,
  CONSTANT,
  COMPILE_ERROR,
  RUNTIME_ERROR
};
}

template<typename T, size_t STACK_SIZE = 256>
struct StaticStack {
  T m_data[STACK_SIZE];
  T* m_data_top;

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

  T peek(int distance)
  {
    return m_data_top[-distance];
  }

  void clear()
  {
    m_data_top = m_data;
  }

  void print()
  {
    for (Value* slot = m_data; slot < m_data_top; slot++) {
      std::cout << "[ " << (*slot).to_str() << " ] ";
    }
    std::cout << "\n";
  }
};

struct VM {
  Chunk* m_chunk;
  uint8_t* m_instruction_ptr;
  StaticStack<Value> m_vm_stack;

  InterpretResult::Any interpret(const std::string& source)
  {
    Chunk chunk;

    std::cerr << "interpet enter\n";
    bool compile_success_flag = compile(source, chunk);
    if (compile_success_flag == false) {
      std::cerr << "compile error\n";
      return InterpretResult::COMPILE_ERROR;
    }
    std::cerr << "interpret exit\n";
    m_chunk = &chunk;
    m_instruction_ptr = m_chunk->m_data;
    InterpretResult::Any result = execute();
    return result;
  }

  InterpretResult::Any interpret(Chunk& chunk)
  {
    m_chunk = &chunk;
    m_instruction_ptr = m_chunk->m_data;
    return execute();
  }

  void init()
  {
    reset_vm_stack();
  }

  InterpretResult::Any execute()
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
          m_vm_stack.pop();
          return InterpretResult::OK;
          break;
        }
        case OpCode::OP_CONSTANT:
        {
          Value constant = read_constant(adv_instruction());
          m_vm_stack.push(constant);
          break;
        }
        case OpCode::OP_TRUE:
        {
          m_vm_stack.push(Value::make<bool>(true));
          break;
        }
        case OpCode::OP_FALSE:
        {
          m_vm_stack.push(Value::make<bool>(false));
          break;
        }
        case OpCode::OP_NIL:
        {
          m_vm_stack.push(Value::make<nullptr_t>(nullptr));
          break;
        }
        case OpCode::OP_NEGATE:
        {
          if (!peek(0).is_type<Number>()) {
            runtime_error("Operand must be a number");
            return InterpretResult::RUNTIME_ERROR;
          }
          m_vm_stack.push(Value::make<Number>(-(m_vm_stack.pop().get<Number>())));
          break;
        }
        case OpCode::OP_NOT:
        {
          m_vm_stack.push(Value::make<bool>(is_value_false(m_vm_stack.pop())));
          break;
        }
        case OpCode::OP_EQUAL:
        {
          Value a = m_vm_stack.pop();
          Value b = m_vm_stack.pop();
          m_vm_stack.push(Value::make<bool>(is_values_equal(a, b)));
          break;
        }
        case OpCode::OP_GREATER:
        {
          break;
        }
        case OpCode::OP_LESS:
        {
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
  InterpretResult::Any binary_op(const Fn& func)
  {
    if (!peek(0).is_type<Number>() || !peek(1).is_type<Number>()) {
      runtime_error("Operand must be numbers");
      return InterpretResult::RUNTIME_ERROR;
    }
    double b = m_vm_stack.pop().as_number();
    double a = m_vm_stack.pop().as_number();
    m_vm_stack.push(Value::make<Number>(func(a, b)));
    return InterpretResult::OK;
  }

  // template<typename ...Args>
  void runtime_error(const std::string& message
    // , Args... args
    , ...
  )
  {
    // std::abort();
    fmt::print(stderr, "{}", message);
    fmt::print(stderr, "\n");

    size_t instruction = m_instruction_ptr - m_chunk->m_data - 1;
    int line = m_chunk->m_lines[instruction];
    fmt::print(stderr, "[line {}] in script\n", line);
    reset_vm_stack();
  }

  Value peek(int distance)
  {
    // note the interpreter advances past each instruction after executing it
    return m_vm_stack.peek(distance - 1);
  }

};
