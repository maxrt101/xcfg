#include "xcfg/value.h"
#include "xcfg/data.h"
#include "xcfg/util/ordered_map.h"
#include "xcfg/util/string.h"

using namespace xcfg;

Value Value::newString(const std::string& s) {
  Value value;
  value.type = Type::STRING;
  value.data = std::shared_ptr<Data>(new StringData(s));
  return value;
}

Value Value::newObject(const OrderedMap<std::string, Value>& items,
                       const OrderedMap<std::string, std::string>& attrs) {
  Value value;
  value.type = Type::OBJECT;
  value.data = std::shared_ptr<Data>(new ObjectData(items, attrs));
  return value;
}

Value Value::newExpr(const std::vector<std::string>& expr) {
  Value value;
  value.type = Type::EXPR;
  value.data = std::shared_ptr<Data>(new ExprData(expr));
  return value;
}

Value Value::empty() {
  Value value;
  value.type = Type::NONE;
  return value;
}

Value Value::emptyString() {
  Value result;
  result.type = Type::STRING;
  result.data = std::shared_ptr<Data>(new StringData());
  return result;
}

Value Value::emptyObject() {
  Value result;
  result.type = Type::OBJECT;
  result.data = std::shared_ptr<Data>(new ObjectData());
  return result;
}

Value Value::emptyExpr() {
  Value value;
  value.type = Type::EXPR;
  value.data = std::shared_ptr<Data>(new ExprData());
  return value;
}

bool Value::isEmpty() const {
  return type == Type::NONE;
}

bool Value::isString() const {
  return type == Type::STRING;
}

bool Value::isObject() const {
  return type == Type::OBJECT;
}

bool Value::isExpr() const {
  return type == Type::EXPR;
}

Data* Value::getData() {
  return data.get();
}

bool Value::has(const std::string& path, const std::string& sep) {
  auto p = strsplit(path, sep);

  Value* value = this;

  if (!value->isObject()) {
    return false;
  }

  for (auto& e : p) {
    if (value->items().has(e)) {
      value = &dynamic_cast<ObjectData*>(value->data.get())->items[e];
    } else {
      return false;
    }
  }

  return true;
}

Value& Value::get(const std::string& path, const std::string& sep) {
  auto p = strsplit(path, sep);

  Value* value = this;

  for (auto& e : p) {
    value = &dynamic_cast<ObjectData*>(value->data.get())->items[e];
  }

  return *value;
}

std::string& Value::string() const {
  return dynamic_cast<StringData*>(data.get())->value;
}

OrderedMap<std::string, Value>& Value::items() const {
  return dynamic_cast<ObjectData*>(data.get())->items;
}

OrderedMap<std::string, std::string>& Value::attrs() const {
  return dynamic_cast<ObjectData*>(data.get())->attrs;
}

Value& Value::operator[](const std::string& key) {
  return dynamic_cast<ObjectData*>(data.get())->items[key];
}

Value Value::clone() {
  Value result;
  result.type = type;
  result.data = data;
  return result;
}
