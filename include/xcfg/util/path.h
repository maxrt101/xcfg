#pragma once

#include <string>

namespace xcfg::path {

bool doesFileExist(const std::string& name);

std::string getFileName(const std::string& path);
std::string stripFileName(const std::string& path);

}
