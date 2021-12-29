#pragma once

#include "scanner.hpp"
#include "chunk.hpp"
#include <fmt/core.h>
#include <functional>

#ifdef DEBUG_PRINT_CODE
#include "debug.hpp"
#endif

namespace Precedence
{

enum Any {
  PREC_NONE, // Lowest
  PREC_ASSIGNMENT, // =
  PREC_OR, // or
  PREC_AND, // and
  PREC_EQUALITY, // == !=
  PREC_COMPARISON, // < > <= >=
  PREC_TERM, //  + -
  PREC_FACTOR, // * /
  PREC_UNARY, // ! -
  PREC_CALL, // . ()
  PREC_PRIMARY // Highest
};

}

void log_error(const std::string& message)
{
  std::cerr << message << "\n";
}


struct Parser {
  Scanner& m_scanner;
  Token m_current;
  Token m_previous;
  Chunk* m_compiling_chunk;

  bool m_error_state = false;
  bool m_panic_mode = false;

  struct ParseRule {
    void (Parser::* m_prefix)();
    void (Parser::* m_infix)();
    Precedence::Any m_precedence;
  };

  ParseRule m_rules[128] = {
        [TokenType::TOKEN_LEFT_PAREN]
          = {&Parser::compile_grouping, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_RIGHT_PAREN]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_LEFT_BRACE]
          = {&Parser::compile_unary, &Parser::compile_binary, Precedence::PREC_TERM},
        [TokenType::TOKEN_RIGHT_BRACE]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_COMMA]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_DOT]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_SEMICOLON]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_MINUS]
          = {&Parser::compile_unary, &Parser::compile_binary, Precedence::PREC_TERM},
        [TokenType::TOKEN_PLUS]
          = {nullptr, &Parser::compile_binary, Precedence::PREC_TERM},
        [TokenType::TOKEN_SLASH]
          = {nullptr, &Parser::compile_binary, Precedence::PREC_FACTOR},
        [TokenType::TOKEN_STAR]
          = {nullptr, &Parser::compile_binary, Precedence::PREC_FACTOR},
        [TokenType::TOKEN_BANG]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_BANG_EQUAL]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_EQUAL]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_EQUAL_EQUAL]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_GREATER]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_GREATER_EQUAL]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_LESS]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_LESS_EQUAL]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_IDENTIFIER]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_STRING]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_NUMBER]
          = {&Parser::compile_number, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_AND]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_CLASS]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_ELSE]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_FALSE]
          = {&Parser::compile_literal, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_FOR]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_FN]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_IF]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_NIL]
          = {&Parser::compile_literal, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_OR]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_PRINT]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_RETURN]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_SUPER]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_THIS]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_TRUE]
          = {&Parser::compile_literal, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_LET]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_WHILE]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_ERROR]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE},
        [TokenType::TOKEN_EOF]
          = {&Parser::compile_unary, nullptr, Precedence::PREC_NONE}
  };

  Parser(Scanner& scanner, Chunk& compiling_chunk)
    : m_scanner{ scanner },
    m_compiling_chunk{ &compiling_chunk }
  {

  }

  ~Parser()
  {
    emit_return();
#ifdef DEBUG_PRINT_CODE
    if (!m_error_state) {
      disassemble_chunk(get_current_chunk(), "code");
    }
#endif
  }

  auto bind_unary()
  {

  }

  auto bind_binary()
  {

  }

  auto bind_number()
  {

  }

  void parse_precedence(Precedence::Any precedence)
  {
    advance();
    void (Parser:: * prefix_rule)() = get_rule(m_previous.m_type)->m_prefix;
    if (prefix_rule == nullptr) {
      log_error("expected expression");
      return;
    }

    (this->*prefix_rule)();
    while (precedence <= get_rule(m_current.m_type)->m_precedence) {
      advance();
      void (Parser:: * infix_rule)() = get_rule(m_previous.m_type)->m_infix;

      (this->*infix_rule)();

    }

  }

  void advance()
  {

    m_previous = m_current;

    while (true) {

      m_current = m_scanner.scan_token();

      std::cerr << m_current.m_type << "\n";
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
    parse_precedence(Precedence::PREC_ASSIGNMENT);
  }

  void compile_grouping()
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

  ParseRule* get_rule(TokenType::Any type)
  {
    return &m_rules[type];
  }

  void compile_unary()
  {
    TokenType::Any operator_type = m_previous.m_type;
    expression(); // TODO
    switch (operator_type) {
      case TokenType::TOKEN_MINUS:
        emit_byte(OpCode::OP_NEGATE);
        break;

      default:
        return;
    }
  }

  void compile_binary()
  {

    TokenType::Any operator_type = m_previous.m_type;
    ParseRule* rule = get_rule(operator_type);
    parse_precedence((Precedence::Any)(rule->m_precedence + 1));

    switch (operator_type) {
      case TokenType::TOKEN_PLUS: emit_byte(OpCode::OP_ADD);
        break;
      case TokenType::TOKEN_MINUS: emit_byte(OpCode::OP_SUBTRACT);
        break;
      case TokenType::TOKEN_STAR: emit_byte(OpCode::OP_MULTIPLY);
        break;
      case TokenType::TOKEN_SLASH: emit_byte(OpCode::OP_DIVIDE);
        break;
      default:
        return; // unreachable
    }
  }

  void compile_literal()
  {

    TokenType::Any operator_type = m_previous.m_type;
    switch (m_previous.m_type) {
      case TokenType::TOKEN_FALSE:
        emit_byte(OpCode::OP_FALSE);
        break;
      case TokenType::TOKEN_TRUE:
        emit_byte(OpCode::OP_TRUE);
        break;
      case TokenType::TOKEN_NIL:
        emit_byte(OpCode::OP_NIL);
        break;
      default:
        return; // unreachable
    }
  }

  void compile_number()
  {
    double value = std::stod(m_previous.m_start);
    emit_constant(Value::make<Number>(value));
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


bool compile(const std::string& source, Chunk& chunk)
{
  Scanner scanner{ source };
  Parser parser{ scanner, chunk };
  parser.advance();
  parser.expression();
  parser.consume(TokenType::TOKEN_EOF, "Expect end of expression.");
  int line = -1;

  return  !parser.m_error_state;
}
