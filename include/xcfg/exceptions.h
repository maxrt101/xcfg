#pragma once

#include <exception>
#include <string>

namespace xcfg {

struct ConfigInvalidExprException : public std::runtime_error {
  ConfigInvalidExprException(const std::string& msg);
};

struct ConfigDefineNotFoundException : public std::runtime_error {
  ConfigDefineNotFoundException(const std::string& msg);
};

struct ConfigIncludeFileNotFoundException : public std::runtime_error {
  ConfigIncludeFileNotFoundException(const std::string& file);
};

}
