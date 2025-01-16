#include "xcfg/data.h"

using namespace xcfg;

StringData::StringData(const std::string& value) : value(value) {}

ObjectData::ObjectData(const OrderedMap<std::string, Value>& items,
                       const OrderedMap<std::string, std::string>& attrs) : items(items), attrs(attrs) {}

ExprData::ExprData(const std::vector<std::string>& expr) : expr(expr) {}