#pragma once

#include <string>
#include "Variable.hpp"

namespace jftt {

enum class Operator {
    none,
    plus,
    minus,
    mul,
    div,
    modulo
};

struct Expression {
    Variable leftValue;
    Operator operat;
    Variable rightValue;
};

} // namespace jftt