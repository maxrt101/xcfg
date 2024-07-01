#pragma once

#include "xcfg/value.h"

namespace xcfg {

class ParserException : public std::exception {
  std::string msg;

public:
  explicit inline ParserException(const std::string& msg) : msg("ParserException: " + msg) {}

  inline ParserException(size_t line, const std::string& msg) {
    this->msg = "ParserException at line " + std::to_string(line) + ": " + msg;
  }

  inline const char * what() const noexcept override {
    return msg.c_str();
  }
};

Value parse(const std::string& text);

} /* namespace xcfg */
