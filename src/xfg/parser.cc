#include "xcfg/parser.h"
#include "xcfg/tokenizer.h"
#include "xcfg/data.h"
#include "xcfg/util/ordered_map.h"
#include "xcfg/util/string.h"

using namespace xcfg;

class Parser {
  const std::vector<Token>& tokens;
  size_t current_idx = 0;

  struct ValueDecl {
    std::string name;
    Value value;
  };

private:
  bool isAtEnd() {
    return current_idx >= tokens.size();
  }

  size_t advance() {
    return ++current_idx;
  }

  bool checkToken(TokenType type, size_t offset = 0) {
    return tokens[current_idx + offset].type == type;
  }

  bool expectToken(TokenType type) {
    if (tokens[current_idx].type == type) {
      advance();
      return true;
    }

    return false;
  }

  Token prevToken() {
    return tokens[current_idx-1];
  }

  Token currentToken() {
    return tokens[current_idx];
  }

  OrderedMap<std::string, std::string> parseAttrs() {
    OrderedMap<std::string, std::string> attrs;

    if (!checkToken(TOKEN_LEFT_SQUARE_BRACE)) {
      throw ParserException(currentToken().line, "Expected '[' (attr list) near '" + currentToken().value + "'");
    }

    advance();

    while (!checkToken(TOKEN_RIGHT_SQUARE_BRACE) && !isAtEnd()) {
      if (!checkToken(TOKEN_STRING)) {
        throw ParserException(currentToken().line, "Expected string (attr list) near '" + currentToken().value + "'");
      }

      std::string name = currentToken().value;
      std::string value;

      advance();

      if (checkToken(TOKEN_EQUAL)) {
        advance();

        if (!checkToken(TOKEN_STRING)) {
          throw ParserException(currentToken().line, "Expected string (attr list) near '" + currentToken().value + "'");
        }

        value = currentToken().value;

        advance();
      }

      attrs[name] = value;

      if (!checkToken(TOKEN_COMMA)) {
        break;
      }

      advance();
    }

    if (!checkToken(TOKEN_RIGHT_SQUARE_BRACE)) {
      throw ParserException(currentToken().line, "Expected ']' (attr list) near '" + currentToken().value + "'");
    }

    advance();

    return attrs;
  }


  ValueDecl parseValue() {
    if (checkToken(TOKEN_DOLLAR)) {
      auto expr = parse_expr();
      return {"${" + strjoin(dynamic_cast<ExprData*>(expr.getData())->expr, " ") + "}", expr};
    }

    if (!checkToken(TOKEN_STRING)) {
      throw ParserException(currentToken().line, "Expected identifier (value decl) near '" + currentToken().value + "'");
    }

    std::string name = currentToken().value;

    advance();

    // Check for single string values
    if (checkToken(TOKEN_COMMA) || checkToken(TOKEN_RIGHT_BRACE)) {
      return {name, Value::newString(name)};
    }

    if (checkToken(TOKEN_EQUAL) || checkToken(TOKEN_COLON)) {
      advance();

      if (checkToken(TOKEN_DOLLAR)) {
        return {name, parse_expr()};
      }

      if (!checkToken(TOKEN_STRING)) {
        throw ParserException(currentToken().line, "Expected string (string decl) near '" + currentToken().value + "'");
      }

      std::string value = currentToken().value;

      advance();

      return {name, Value::newString(value)};
    }

    OrderedMap<std::string, std::string> attrs;
    OrderedMap<std::string, Value> items;

    if (checkToken(TOKEN_LEFT_SQUARE_BRACE)) {
      attrs = parseAttrs();
    }

    if (!checkToken(TOKEN_LEFT_BRACE)) {
      throw ParserException(currentToken().line, "Expected '{' (value decl) near '" + currentToken().value + "'");
    }

    advance();

    while (!checkToken(TOKEN_RIGHT_BRACE) && !isAtEnd()) {
      auto value_decl = parseValue();
      items[value_decl.name] = value_decl.value;

      // Exit obj decl if next token is '}' - it means we are at the end of obj decl
      if (checkToken(TOKEN_RIGHT_BRACE)) {
        break;
      }

      // Check for delimiter (',' ';'), if found none - exit obj decl
      // Delimiter is not needed, if prev value was as object (so to not put ',' after '}')
      if (!checkToken(TOKEN_SEMICOLON) && !checkToken(TOKEN_COMMA) && prevToken().type != TOKEN_RIGHT_BRACE) {
        break;
      }

      // If there is a delimiter (',' ';') - skip it
      // Also skip '}' at the end of expr, if last parsed value was an expr
      if (prevToken().type != TOKEN_RIGHT_BRACE || value_decl.value.isExpr()) {
        advance();
      }
    }

    if (!checkToken(TOKEN_RIGHT_BRACE)) {
      throw ParserException(currentToken().line, "Expected '}' (object decl) near '" + currentToken().value + "'");
    }

    advance();

    return {name, Value::newObject(items, attrs)};
  }

  Value parse_expr() {
    std::vector<std::string> expr;

    if (!checkToken(TOKEN_DOLLAR)) {
      throw ParserException(currentToken().line, "Expected '$' (expr) near '" + currentToken().value + "'");
    }

    advance();

    if (!checkToken(TOKEN_LEFT_BRACE)) {
      throw ParserException(currentToken().line, "Expected '{' (expr) near '" + currentToken().value + "'");
    }

    advance();

    while (!checkToken(TOKEN_RIGHT_BRACE) || isAtEnd()) {
      expr.push_back(currentToken().value);
      advance();
    }

    if (!checkToken(TOKEN_RIGHT_BRACE)) {
      throw ParserException(currentToken().line, "Expected '}' (expr) near '" + currentToken().value + "'");
    }

    advance();

    return Value::newExpr(expr);
  }

public:
  explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

  Value parse() {
    OrderedMap<std::string, Value> items;

    while (!isAtEnd()) {
      auto value_decl = parseValue();

      items[value_decl.name] = value_decl.value;
    }

    return Value::newObject(items);
  }
};

Value xcfg::parse(const std::string& text) {
  auto tokens = tokenize(text);

  return Parser(tokens).parse();
}
