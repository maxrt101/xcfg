#pragma once

#include "xcfg/xcfg.h"
#include <vector>
#include <map>

namespace xcfg {

struct Env {
  std::map<std::string, std::string> defines;
  std::vector<std::string> includePaths;
};

void process(Value& root, Value& value, Env& env);

}
