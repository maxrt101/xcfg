#include "xcfg/processor.h"
#include "xcfg/exceptions.h"
#include "xcfg/util/string.h"
#include "xcfg/util/path.h"

#include <fstream>
#include <sstream>

using namespace xcfg;

struct EvalCtx {
  Env& env;
  Value& root;
  enum Stage {
    STAGE1, STAGE2, STAGE3, STAGE4,
    STAGE_MAX
  } stage;
};

static void eval(EvalCtx& ctx, Value& parent, std::string key, Value& value) {
  std::vector<std::string> expr = dynamic_cast<ExprData*>(value.getData())->expr;

  if (expr.empty()) {
    parent.items().remove(key);
  }

  switch (ctx.stage) {
    case EvalCtx::STAGE1:
      if (expr[0] == "include") {
        if (expr.size() != 2) {
          throw ConfigInvalidExprException(strjoin(expr, " "));
        }

        std::ifstream file(expr[1]);

        if (!file.is_open()) {
          for (std::string& folder : ctx.env.includePaths) {
            std::string fileName = folder;
            if (folder.back() != '/') {
              fileName += "/";
            }
            fileName += expr[1];
            if (path::doesFileExist(fileName)) {
              file = std::ifstream(fileName);
            }
          }
        }

        if (!file.is_open()) {
          throw ConfigIncludeFileNotFoundException(expr[1]);
        }

        std::stringstream ss;
        ss << file.rdbuf();

        auto result = xcfg::parse(ss.str());
        for (auto& k : result.items()) {
          parent.items().append(k, result[k]);
        }
        parent.items().remove(key);
      }
      break;

    case EvalCtx::STAGE2:
      if (expr[0] == "define") {
        if (expr.size() != 3) {
          throw ConfigInvalidExprException("Malformed 'define': " + strjoin(expr, " "));
        }
        ctx.env.defines[expr[1]] = expr[2];
        parent.items().remove(key);
      }
      break;

    case EvalCtx::STAGE3:
      if (expr[0] == "insert" || expr[0] == "include" || expr[0] == "define") {
        return;
      }
      if (ctx.env.defines.find(expr[0]) == ctx.env.defines.end()) {
        throw ConfigDefineNotFoundException(expr[0]);
      }
      value = Value::newString(ctx.env.defines[expr[0]]);
      break;

    case EvalCtx::STAGE4:
      if (expr[0] == "insert") {
        if (expr.size() != 2) {
          throw ConfigInvalidExprException("Malformed 'insert': " + strjoin(expr, " "));
        }
        if (expr[1].empty()) {
          throw ConfigInvalidExprException("Malformed 'insert' (empty path): " + strjoin(expr, " "));
        }
        auto& object = ctx.root.get(expr[1]);
        for (auto& k : object.items()) {
          parent.items().append(k, object[k]);
        }
        parent.items().remove(key);
      }
      break;

    default:
      break;
  }
}

static void processValue(EvalCtx& ctx, Value& parent, std::string key, Value& value) {
  if (value.isEmpty()) {
    parent.items().remove(key);
  }

  if (value.isExpr()) {
    eval(ctx, parent, key, value);
  }

  if (value.isObject()) {
    auto& items = value.items();

    for (auto& k : items) {
      processValue(ctx, value, k, items[k]);
    }
  }
}


void xcfg::process(Value& root, Value& value, Env& env) {
  EvalCtx ctx = {
    .env = env,
    .root = root,
  };

  for (uint8_t stage = EvalCtx::STAGE1; stage < EvalCtx::STAGE_MAX; ++stage) {
    ctx.stage = (EvalCtx::Stage) stage;
    processValue(ctx, root, "", value);
  }
}