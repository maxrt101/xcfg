#pragma once

#include <vector>
#include <string>

namespace xcfg {

enum TokenType {
  TOKEN_STRING,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_LEFT_SQUARE_BRACE,
  TOKEN_RIGHT_SQUARE_BRACE,
  TOKEN_EQUAL,
  TOKEN_COMMA,
  TOKEN_SEMICOLON,
  TOKEN_COLON,
  TOKEN_DOLLAR
};

struct Token {
  TokenType type;
  std::string value;
  size_t line;
};

class TokenizerException : public std::exception {
  std::string msg;

public:
  explicit inline TokenizerException(const std::string& msg) : msg("TokenizerException: " + msg) {}

  inline TokenizerException(size_t line, const std::string& msg) {
    this->msg = "TokenizerException at line " + std::to_string(line) + ": " + msg;
  }

  inline const char * what() const noexcept override {
    return msg.c_str();
  }
};

std::vector<Token> tokenize(const std::string& text);

}