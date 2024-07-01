#pragma once

#include <functional>
#include <vector>
#include <string>

namespace xcfg {

std::vector<std::string> strsplit(const std::string& str, const std::string& delimiter = " ");
std::string strstrip(const std::string& str, char c = ' ');

template <typename T>
std::string strjoin(const std::vector<T>& vec, const std::string& sep = ", ", std::function<std::string(T)> stringifier = [](T val) { return val; }) {
  std::string result;
  for (size_t i = 0; i < vec.size(); i++) {
    result += stringifier(vec[i]);
    if (i + 1 < vec.size()) {
      result += sep;
    }
  }
  return result;
}

}