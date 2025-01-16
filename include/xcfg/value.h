#pragma once

#include <memory>
#include <string>
#include <vector>

#include "xcfg/util/ordered_map.h"

namespace xcfg {

struct Data;

enum class Type : uint8_t {
  NONE = 0,
  STRING,
  OBJECT,
  EXPR,
};

struct Value {
  Type type = Type::NONE;
  std::shared_ptr<Data> data;

  Value() = default;
  ~Value() = default;

  static Value newString(const std::string& s);
  static Value newObject(const OrderedMap<std::string, Value>& items,
                         const OrderedMap<std::string, std::string>& attrs = {});

  static Value newExpr(const std::vector<std::string>& expr);

  static Value empty();
  static Value emptyString();
  static Value emptyObject();
  static Value emptyExpr();

  bool isEmpty() const;
  bool isString() const;
  bool isObject() const;
  bool isExpr() const;

  Data* getData();

  bool has(const std::string& path, const std::string& sep = ".");
  Value& get(const std::string& path, const std::string& = ".");

  std::string& string() const;

  OrderedMap<std::string, Value>& items() const;
  OrderedMap<std::string, std::string>& attrs() const;

  Value& operator[](const std::string& key);

  Value clone();
};

}
