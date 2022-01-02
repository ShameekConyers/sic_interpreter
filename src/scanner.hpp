#pragma once

#include <string>

namespace TokenType
{
enum Any {
  // single-char tokens 0
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_COMMA,
  TOKEN_DOT,
  TOKEN_SEMICOLON,
  TOKEN_MINUS,
  TOKEN_PLUS,
  TOKEN_SLASH,
  TOKEN_STAR,

  // one or two character tokens 11
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,

  // Literals 19
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,

  // Keywords 22
  TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
  TOKEN_FOR, TOKEN_FN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
  TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
  TOKEN_TRUE, TOKEN_LET, TOKEN_WHILE,

  // Misc 38
  TOKEN_ERROR, TOKEN_EOF
};
}

struct Token {
  TokenType::Any m_type;
  const char* m_start;
  uint32_t m_length;
  uint32_t m_line;

};

char* make_token_error_msg(const std::string& message)
{
  // HERE
  char* error_message = new char[message.size() + 1];

  for (uint32_t i = 0; i < message.size(); i++) {
    error_message[i] = message[i];
  }
  error_message[message.size()] = '\0';
  return error_message;
}

void process_token_error_msg(Token& token)
{
  delete [] token.m_start;
  token.m_start = nullptr;
}


struct Scanner {
  // m_start_lexeme_char
  const char* m_start;
  // m_current_lexeme_char (end of scanned token)
  const char* m_current;
  uint32_t m_line;


  Scanner(const std::string& source)
  {
    m_start = source.data();
    m_current = source.data();
    m_line = 0;
  }

  Token scan_token()
  {
    // std::cerr << "enter scan token\n";
    skip_whitespace();
    m_start = m_current;

    if (is_at_end()) {
      return make_token(TokenType::TOKEN_EOF);
    }
    char c = advance();

    if (is_alpha(c)) return make_identifier_token();
    if (is_digit(c)) return make_number_token();
    switch (c) {
      case '(': return make_token(TokenType::TOKEN_LEFT_PAREN);
      case ')': return make_token(TokenType::TOKEN_RIGHT_PAREN);
      case '{': return make_token(TokenType::TOKEN_LEFT_BRACE);
      case '}': return make_token(TokenType::TOKEN_RIGHT_BRACE);
      case ';': return make_token(TokenType::TOKEN_SEMICOLON);
      case ',': return make_token(TokenType::TOKEN_COMMA);
      case '.': return make_token(TokenType::TOKEN_DOT);
      case '-': return make_token(TokenType::TOKEN_MINUS);
      case '+': return make_token(TokenType::TOKEN_PLUS);
      case '/': return make_token(TokenType::TOKEN_SLASH);
      case '*': return make_token(TokenType::TOKEN_STAR);
      case '!':
        return make_token(
          match('=') ? TokenType::TOKEN_BANG_EQUAL
          : TokenType::TOKEN_BANG);
      case '=':
        return make_token(
          match('=') ? TokenType::TOKEN_EQUAL_EQUAL
          : TokenType::TOKEN_EQUAL);
      case '<':
        return make_token(
          match('=') ? TokenType::TOKEN_LESS_EQUAL
          : TokenType::TOKEN_LESS);
      case '>':
        return make_token(
          match('=') ? TokenType::TOKEN_GREATER_EQUAL
          : TokenType::TOKEN_GREATER);
      case '"':
        return make_string_token();
    }



    return make_error_token("unexpected character.");
  }

  bool is_alpha(char c)
  {
    return ('a' <= c && c <= 'z') ||
      (c >= 'A' && c <= 'Z') ||
      c == '_';
  }

  bool is_digit(char c)
  {
    return '0' <= c && c <= '9';
  }

  // conditionally consumes the next character
  bool match(char input)
  {
    if (is_at_end()
      || *m_current != input) {

      return false;
    }

    m_current++;
    return true;
  }

  char advance()
  {
    m_current++;
    return m_current[-1];
  }

  char peek()
  {
    return m_current[0];
  }

  char peek_next()
  {
    if (is_at_end()) return '\0';
    return m_current[1];
  }

  void skip_whitespace()
  {
    while (true) {
      char c = peek();
      switch (c) {
        case '\n':
          m_line++;
        case ' ':
        case '\r':
        case '\t':
          advance();
          break;
        case '/':
          if (peek_next() == '/') {
            while (peek() != '\n' && !is_at_end()) advance();
          }
          else {
            return; // not whitespace
          }
          break;
        default:
          return;
      }
    }
  }

  bool is_at_end()
  {

    return *m_current == '\0'; // HERE
  }

  Token make_token(TokenType::Any type)
  {
    Token token;
    token.m_type = type;
    token.m_start = m_start;
    token.m_length = (uint32_t)(m_current - m_start);
    token.m_line = m_line;
    return token;
  }

  Token make_error_token(const std::string& message)
  {
    Token token;
    token.m_type = TokenType::TOKEN_ERROR;
    token.m_start = make_token_error_msg(message);
    token.m_length = (uint32_t)message.size();
    token.m_line = m_line;
    return token;
  }

  Token make_string_token()
  {

    while (peek() != '"' && !is_at_end()) {
      if (peek() == '\n') {
        m_line++;
      }
      advance();
    }

    if (is_at_end()) {
      return make_error_token("Unterminated String.");
    }

    advance();
    return make_token(TokenType::TOKEN_STRING);
  }

  Token make_number_token()
  {
    while (is_digit(peek())) {
      advance();
    }

    if (peek() == '.' && is_digit(peek_next())) {
      advance();
      while (is_digit(peek())) {
        advance();
      }
    }
    return make_token(TokenType::TOKEN_NUMBER);
  }

  Token make_identifier_token()
  {
    while (is_alpha(peek()) || is_digit(peek())) {
      advance();
    }

    return make_token(find_identifier_type());
  }

  // Trie
  TokenType::Any find_identifier_type()
  {
    switch (m_start[0]) {
      case 'a': return check_keyword("and", TokenType::TOKEN_AND);
      case 'c': return check_keyword("class", TokenType::TOKEN_CLASS);
      case 'e': return check_keyword("else", TokenType::TOKEN_ELSE);
      case 'i': return check_keyword("if", TokenType::TOKEN_IF);
      case 'n': return check_keyword("nil", TokenType::TOKEN_NIL);
      case 'o': return check_keyword("or", TokenType::TOKEN_OR);
      case 'p': return check_keyword("print", TokenType::TOKEN_PRINT);
      case 'r': return check_keyword("return", TokenType::TOKEN_RETURN);
      case 's': return check_keyword("super", TokenType::TOKEN_SUPER);
      case 'l': return check_keyword("let", TokenType::TOKEN_LET);
      case 'w': return check_keyword("while", TokenType::TOKEN_WHILE);

      case 'f':
        if (m_current - m_start == 1) {
          break;
        }
        switch (m_start[1]) {
          case 'n': return TokenType::TOKEN_FN;
          case 'o': return check_keyword("for", TokenType::TOKEN_FOR);
          case 'a': return check_keyword("false", TokenType::TOKEN_FALSE);
        }

      case 't':
        if (m_current - m_start == 1) {
          break;
        }
        switch (m_start[1]) {
          case 'r': return check_keyword("true", TokenType::TOKEN_TRUE);
          case 'h': return check_keyword("this", TokenType::TOKEN_THIS);
        }

      default:
        break;
    }
    return TokenType::TOKEN_IDENTIFIER;
  }

  TokenType::Any check_keyword(
    const std::string& target_keyword,
    TokenType::Any token_to_check)
  {

    const char* cursor = m_start;
    size_t keyword_size = target_keyword.size();
    if (m_current - m_start != keyword_size) {

      return TokenType::TOKEN_IDENTIFIER;
    }

    for (uint32_t i = 0; i < keyword_size; i++) {
      if (target_keyword[i] != cursor[i]) {
        return TokenType::TOKEN_IDENTIFIER;
      }
    }

    return token_to_check;
  }

};
