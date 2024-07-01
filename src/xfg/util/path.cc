#include "xcfg/util/path.h"
#include "xcfg/util/string.h"
#include <sys/stat.h>

bool xcfg::path::doesFileExist(const std::string& name) {
  struct stat buffer;
  return stat(name.c_str(), &buffer) == 0;
}

std::string xcfg::path::getFileName(const std::string& path) {
  auto pos = path.rfind('/');
  if (pos == path.npos) {
    return path;
  }
  return path.substr(pos);
}

std::string xcfg::path::stripFileName(const std::string& path) {
  auto pos = path.rfind('/');
  if (pos == path.npos) {
    return "./";
  }
  return path.substr(0, pos);
}
