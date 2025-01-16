#include "xcfg/xcfg.h"
#include "xcfg/util/string.h"

#include <stdio.h>

using namespace xcfg;

constexpr int INDENT_INC = 2;

static inline void printIndent(size_t indent) {
  while (indent) {
    putchar(' ');
    --indent;
  }
}

static inline void printAttrs(const OrderedMap<std::string, std::string>& attrs) {
  if (!attrs.size()) return;

  printf(" [");

  size_t index = 0;
  for (auto& k : attrs) {
    printf("%s", k.c_str());

    if (!attrs[k].empty()) {
      printf(" = %s", attrs[k].c_str());
    }

    if (index + 1 < attrs.size()) {
      printf(", ");
    }

    ++index;
  }

  putchar(']');
}

static inline void printTrailing(Type type, Type parent_type) {
  switch (parent_type) {
    case Type::OBJECT:
      if (type != Type::OBJECT && type != Type::EXPR) {
        printf(";\n");
      }
      break;
    case Type::EXPR:
    case Type::STRING:
    case Type::NONE:
    default:
      break;
  }
}

void xcfg::prettyPrint(Value& value, size_t indent, Type parent_type, bool is_root) {
  switch (value.type) {
    case Type::STRING:
      printf(" = %s", value.string().c_str());
      break;

    case Type::EXPR:
      printf(
          "${%s}%c\n",
          strjoin(dynamic_cast<ExprData*>(value.getData())->expr, " ").c_str(),
          indent == INDENT_INC ? ' ' : ',' // if indent is INDENT_INC - it means we are in root object
      );
      break;

    case Type::OBJECT:
      if (!is_root) {
        printAttrs(value.attrs());
        printf(" {\n");
      }
      for (auto& k : value.items()) {
        if (!is_root) {
          printIndent(indent);
        }
        if (!value[k].isExpr()) {
          printf("%s", k.c_str());
        }
        if (value[k].isObject() && value[k].items().empty()) {
          printf(" {}\n");
          break;
        }
        if (value[k].isString() && k == value[k].string()) {
          printf(",\n");
          continue;
        }
        prettyPrint(value[k], indent + 2, Type::OBJECT, false);
      }
      if (!is_root) {
        printIndent(indent - 2);
        printf("}\n");
      }
      break;

    case Type::NONE:
    default:
      printf("<unknown type=%d>", (uint8_t) value.type);
      break;
  }
  printTrailing(value.type, parent_type);
}
