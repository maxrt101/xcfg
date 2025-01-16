#include "xcfg/tokenizer.h"
#include <map>

using namespace xcfg;

struct TokenPattern {
  TokenType type;
  std::string text;
  bool is_one_char;

  TokenPattern(TokenType type, const std::string& text) : type(type), text(text) {
    is_one_char = text.size() == 1;
  }
};

static const std::map<char, TokenPattern> token_patterns = {
    {'{', TokenPattern {TOKEN_LEFT_BRACE, "{"}},
    {'}', TokenPattern {TOKEN_RIGHT_BRACE, "}"}},
    {'[', TokenPattern {TOKEN_LEFT_SQUARE_BRACE, "["}},
    {']', TokenPattern {TOKEN_RIGHT_SQUARE_BRACE, "]"}},
    {'=', TokenPattern {TOKEN_EQUAL, "="}},
    {',', TokenPattern {TOKEN_COMMA, ","}},
    {';', TokenPattern {TOKEN_SEMICOLON, ";"}},
    {':', TokenPattern {TOKEN_COLON, ":"}},
    {'$', TokenPattern {TOKEN_DOLLAR, "$"}},
};

class Tokenizer {
private:
  const std::string& text;
  size_t current = 0;
  size_t line = 1;
  std::vector<Token> result;

private:
  bool is_at_end() {
    return current >= text.size();
  }

  size_t consume() {
    return ++current;
  }

  bool is_char_identifier(char c) {
    return isalnum(c) || c == '_' || c == '.' || c == '/';
  }

  void skip_whitespace() {
    while (text[current] == ' ' || text[current] == '\n' || text[current] == '\t' || text[current] == '#') {
      if (is_at_end()) {
        return;
      }

      if (text[current] == '\n') {
        ++line;
      }

      if (text[current] == '#') {
        while (!is_at_end() && text[current] != '\n') {
          consume();
        }
        ++line;
      }

      consume();
    }
  }

  void tokenize_string() {
    size_t start = consume();
    while (text[current] != '"') {
      if (is_at_end()) {
        throw TokenizerException(line, "Unexpected EOF");
      }

      if (text[current] == '\n') {
        ++line;
      }

      consume();
    }

    result.push_back({TOKEN_STRING, text.substr(start, current - start)});

    // Skip closing quote
    consume();
  }

  void tokenize_identifier() {
    size_t begin = current;

    while (is_char_identifier(text[current])) {
      if (is_at_end()) {
        throw TokenizerException(line, "Unexpected EOF");
      }

      consume();
    }

    result.push_back({TOKEN_STRING, text.substr(begin, current - begin), line});
  }

public:
  explicit Tokenizer(const std::string& text) : text(text) {}

  std::vector<Token> tokenize() {

    while (!is_at_end()) {
      skip_whitespace();

      if (is_at_end()) {
        break;
      }

      // FIXME: Add numbers

      if (token_patterns.find(text[current]) != token_patterns.end()) {
        auto pattern = token_patterns.at(text[current]);
        if (pattern.is_one_char) {
          result.push_back({pattern.type, pattern.text, line});
          ++current;
          continue;
        } else {
          if (text.substr(current, pattern.text.size()) == pattern.text
              && !is_char_identifier(text[current + pattern.text.size()])) { // FIXME: Check next char
            result.push_back({pattern.type, pattern.text, line});
            current += pattern.text.size();
            continue;
          }
        }
      }

      if (is_char_identifier(text[current])) {
        tokenize_identifier();
        continue;
      } else if (text[current] == '"') {
        tokenize_string();
        continue;
      }

      throw TokenizerException(line, "Unexpected char (pos=" + std::to_string(current) + ")");
    }

    return result;
  }
};

std::vector<Token> xcfg::tokenize(const std::string& text) {
  return Tokenizer(text).tokenize();
}
