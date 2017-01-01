#pragma once

#include <string>

namespace jftt {

enum class compare {
    eq,             // =
    notEq,          // <>
    lowerThan,      // <
    biggerThan,     // >
    lowerOrEqThan,  // <=
    biggerOrEqThan  // >=
};

struct Condition {
    std::string leftValue;
    compare sign;
    std::string rightValue;
};

} // namespace jftt