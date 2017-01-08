#include "Expression.hpp"

namespace jftt {

std::string operator +(const std::string &txt, Operator oper) {

    switch (oper) {
        case Operator::none:
            return txt;
        case Operator::plus:
            return txt + " + ";
        case Operator::minus:
            return txt + " - ";
        case Operator::mul:
            return txt + " * ";
        case Operator::div:
            return txt + " / ";
        case Operator::modulo:
            return txt + " % ";
    }
}

} // namespace jftt