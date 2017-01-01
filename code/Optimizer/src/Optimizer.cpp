#include "Optimizer.hpp"
#include "OptimizerScanner.hpp"
#include "debug.hpp"

namespace optimizer {

void Optimizer::run(std::istream& in) {
    auto scanner = std::make_shared<Scanner>(in, out, Logger::out);
    auto parser = std::make_shared<LexParser>(*scanner, *this);
    DEBUG << "starting optimizing\n";
    if (parser->parse())
        error << "Parse failed!!\n";
    DEBUG << "ended optimizing\n";
}


void Optimizer::ifCommand(jftt::Condition condition,
                          const std::string &ifCommands,
                          const std::string &elseCommands) {
    constexpr auto registerValue = "$reg1";
    out << registerValue << " = " << condition.leftValue << " - " << condition.rightValue << "\n";
    out << "JZERO 1 %ELSE" << numOfIf << "%\n";
    out << ifCommands << "\n";
    out << "JUMP %ENDIF" << numOfIf << "%\n";
    out << "%ELSE" << numOfIf << "%: ";
    out << elseCommands << "\n";
    out << "%ENDIF" << numOfIf << "%: ";
    ++numOfIf;
}

Optimizer::Optimizer(std::ostream &out) : out(out) {}

std::ostream &Optimizer::getOut() const {
    return out;
}

} // namespace optimizer