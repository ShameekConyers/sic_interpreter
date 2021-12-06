#pragma once

#include "scanner.hpp"
#include "chunk.hpp"
#include <fmt/core.h>

struct Parser {
  Scanner& m_scanner;
  Token m_current;
  Token m_previous;
  Chunk* m_compiling_chunk;

  bool m_error_state = false;
  bool m_panic_mode = false;

  Parser(Scanner& scanner)
    : m_scanner{ scanner }
  {

  }


  void advance()
  {
    m_previous = m_current;

    while (true) {
      m_current = m_scanner.scan_token();
      if (m_current.m_type != TokenType::TOKEN_ERROR) break;
      else get_error_at_token(m_current, m_current.m_start);
    }

  }

  void consume(TokenType::Any type, const std::string& message)
  {
    if (m_current.m_type == type) {
      advance();
      return;
    }
    else {
      get_error_at_token(m_current, message);
    }
  }

  void emit_byte(uint8_t byte)
  {
    get_current_chunk().write(byte, m_previous.m_line);
  }

  void emit_two_bytes(uint8_t byte_1, uint8_t byte_2)
  {
    emit_byte(byte_1);
    emit_byte(byte_2);
  }

  void emit_return()
  {
    emit_byte(OpCode::OP_RETURN);
  }

  void emit_constant(Value value)
  {
    emit_two_bytes(OpCode::OP_CONSTANT, make_constant(value));
  }

  Chunk& get_current_chunk()
  {
    return *m_compiling_chunk;
  }

  void expression()
  {

  }

  void grouping()
  {
    expression();
    consume(TokenType::TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
  }

  uint8_t make_constant(Value value)
  {
    int idx_of_constant = get_current_chunk().add_constant(value);
    if (idx_of_constant > UINT8_MAX) {
      std::cerr << ("Too many constants in one chunk");
      return 0;
    }

    return (uint8_t)idx_of_constant;
  }

  void compile_unary()
  {
    TokenType::Any operator_type = m_previous.m_type;
    expression();
    switch (operator_type) {
      case TokenType::TOKEN_MINUS:
        emit_byte(OpCode::OP_NEGATE);
        break;

      default:
        return;
    }
  }

  void compile_number()
  {
    double value = std::stod(m_previous.m_start);
    emit_constant(value);
  }

  void get_error_at_token(const Token& token, const std::string& message)
  {

    if (m_panic_mode) return;
    m_panic_mode = true;

    // const char* message = token.m_start;
    fmt::print(stderr, "[line {}] Error ", token.m_line);

    if (token.m_type == TokenType::TOKEN_EOF) {
      fmt::print(stderr, "at end ");
    }
    else if (token.m_type == TokenType::TOKEN_ERROR) {
      // Do nothing?
    }
    else {
      fmt::print(stderr, "at {:.{}s}", token.m_start, token.m_length);
    }

    fmt::print(stderr, ": {}\n", message);
    m_error_state = true;
  }


};


bool compile(const std::string& source, const Chunk& chunk)
{
  Scanner scanner{ source };
  Parser parser{ scanner };

  int line = -1;

  return  parser.m_error_state;
}
