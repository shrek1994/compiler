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


std::string Optimizer::ifCommand(jftt::Condition condition,
                          const std::string &ifCommands,
                          const std::string &elseCommands) {
    std::string command;
    std::string registerValue = "$reg1";
    command += registerValue + " := " + condition.leftValue + " - " + condition.rightValue + ";\n";
    command += std::string("JZERO 1 %ELSE") + std::to_string(numOfIf) + "%;\n";
    command += ifCommands;
    command += std::string("JUMP %ENDIF") + std::to_string(numOfIf) + "%;\n";
    command += std::string("%ELSE") + std::to_string(numOfIf) + "%:";
    command += elseCommands;
    command += std::string("%ENDIF") + std::to_string(numOfIf) + "%:";
    ++numOfIf;
    return command;
}

Optimizer::Optimizer(std::ostream &out) : out(out) {}

std::ostream &Optimizer::getOut() const {
    return out;
}

} // namespace optimizer