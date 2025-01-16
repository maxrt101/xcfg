#pragma once

#include "xcfg/value.h"
#include "xcfg/data.h"
#include "xcfg/tokenizer.h"
#include "xcfg/parser.h"
#include "xcfg/processor.h"

namespace xcfg {

using Config = Value;

void prettyPrint(Value& value, size_t indent = 0, Type parent_type = Type::OBJECT, bool is_root = true);

}
