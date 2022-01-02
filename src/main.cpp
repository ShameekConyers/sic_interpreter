
#include "common.hpp"
#include "chunk.hpp"
#include "debug.hpp"
#include "vm.hpp"


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>



void static repl()
{
  char line[1024];
  for (;;) {
    std::cout << "> ";
    if (!std::cin.getline(line, 1024).good()) {
      std::cout << "\n";
      break;
    }
    vm.interpret(line);
  }
}

std::string static read_file(const std::string& file_path)
{
  std::ifstream in_file;
  in_file.open(file_path);
  if (in_file.fail()) {
    std::cout << "failed to read file: " << file_path << "\n";
    exit(74);
  }
  std::stringstream buffer;
  buffer << in_file.rdbuf();
  return buffer.str();
}

void static run_file(const std::string& file_path)
{
  std::string source = read_file(file_path);
  InterpretResult::Any result = vm.interpret(source);

  if (result == InterpretResult::COMPILE_ERROR) exit(65);
  if (result == InterpretResult::RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv [])
{

  // Chunk chunk;
  // int idx_of_constant;
  // idx_of_constant = chunk.add_constant(1.2);
  // chunk.write(OpCode::OP_CONSTANT, 1);
  // chunk.write(idx_of_constant, 1);

  // idx_of_constant = chunk.add_constant(3.4);
  // chunk.write(OpCode::OP_CONSTANT, 1);
  // chunk.write(idx_of_constant, 1);

  // chunk.write(OpCode::OP_ADD, 1);

  // idx_of_constant = chunk.add_constant(5.6);
  // chunk.write(OpCode::OP_CONSTANT, 1);
  // chunk.write(idx_of_constant, 1);

  // chunk.write(OpCode::OP_DIVIDE, 1);

  // chunk.write(OpCode::OP_NEGATE, 1);
  // chunk.write(OpCode::OP_RETURN, 1);

  // disassemble_chunk(chunk, "test");
  // vm.interpret(chunk);

  // chunk.free();

  if (argc == 1) {
    repl();
  }
  else if (argc == 2) {
    run_file(argv[1]);
  }
  else {
    std::cerr << "Usage: cpplox [path]\n";
    exit(64);
  }
  return 0;
}
