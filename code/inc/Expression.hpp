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

std::string operator+(const std::string& txt, Operator oper);

struct Expression {
    Variable leftValue;
    Operator operat;
    Variable rightValue = {};
};

} // namespace jftt