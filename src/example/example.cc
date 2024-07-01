#include "xcfg/xcfg.h"
#include "xcfg/util/path.h"

#include <fstream>
#include <sstream>

int main(int argc, char ** argv) {
  if (argc == 2) {
    std::ifstream file(argv[1]);
    std::stringstream ss;
    ss << file.rdbuf();

    auto result = xcfg::parse(ss.str());

    xcfg::Env env;

    env.includePaths.push_back(xcfg::path::stripFileName(argv[1]));

    xcfg::process(result, result, env);

    xcfg::prettyPrint(result);

    return 0;
  }

  printf("Usage: %s FILE\n", argv[0]);
  return 1;
}