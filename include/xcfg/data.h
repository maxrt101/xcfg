#pragma once

#include <string>
#include <vector>

#include "xcfg/value.h"
#include "xcfg/util/ordered_map.h"

namespace xcfg {

struct Data {
  virtual ~Data() = default;
};

struct StringData : public Data {
  std::string value;

  StringData() = default;
  ~StringData() override = default;

  explicit StringData(const std::string& value);
};

struct ObjectData : public Data {
  OrderedMap<std::string, Value> items;
  OrderedMap<std::string, std::string> attrs;

  ObjectData() = default;
  ~ObjectData() override = default;

  ObjectData(const OrderedMap<std::string, Value>& items,
             const OrderedMap<std::string, std::string>& attrs = {});
};

struct ExprData : public Data {
  std::vector<std::string> expr;

  ExprData() = default;
  ~ExprData() override = default;

  ExprData(const std::vector<std::string>& expr);
};


/*
 * ObjectData -> FunctionData
 *   attrs -> args
 *   items -> body
 *
 * FunctionData
 *   args
 *   body
 *
 * body: value(call), func, vardecl, assignment, nested_value(x.y.z)
 *
 * value(call): ObjectData
 * func: FunctionData {code(vector<Value>)}
 * vardecl: VardeclData {name, value(nested_value|string|call)}
 * assignment: VardeclData {name, value}
 * nested_value(x.y.z): vector<string>
 * */

}
