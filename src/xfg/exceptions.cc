#include "xcfg/exceptions.h"

xcfg::ConfigInvalidExprException::ConfigInvalidExprException(const std::string& msg)
  : std::runtime_error("Invalid Expression: " + msg) {}

xcfg::ConfigDefineNotFoundException::ConfigDefineNotFoundException(const std::string& msg)
  : std::runtime_error("Define Not Found: " + msg) {}


xcfg::ConfigIncludeFileNotFoundException::ConfigIncludeFileNotFoundException(const std::string& file)
  : std::runtime_error("Include File Not Found: " + file) {}

