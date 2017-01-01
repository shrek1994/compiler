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
    switch (condition.sign) {
        case jftt::compare::eq:
            return std::move(generateEq(condition.leftValue,
                                        condition.rightValue,
                                        ifCommands,
                                        elseCommands));
        case jftt::compare::notEq:
            return std::move(generateEq(condition.leftValue,
                                        condition.rightValue,
                                        elseCommands,
                                        ifCommands));
        case jftt::compare::biggerThan:
            return std::move(generateBiggerThan(condition.leftValue,
                                                condition.rightValue,
                                                ifCommands,
                                                elseCommands));
        case jftt::compare::lowerThan:
            return std::move(generateBiggerThan(condition.rightValue,
                                                condition.leftValue,
                                                ifCommands,
                                                elseCommands));
        case jftt::compare::lowerOrEqThan:
            return std::move(generateBiggerThan(condition.leftValue,
                                                condition.rightValue,
                                                elseCommands,
                                                ifCommands));
        case jftt::compare::biggerOrEqThan:
            return std::move(generateBiggerThan(condition.rightValue,
                                                condition.leftValue,
                                                elseCommands,
                                                ifCommands));
        default:
            break;
    }
}

Optimizer::Optimizer(std::ostream &out) : out(out) {}

std::ostream &Optimizer::getOut() const {
    return out;
}

std::string Optimizer::generateBiggerThan(const std::string &leftValue,
                                          const std::string &rightValue,
                                          const std::string &ifCommands,
                                          const std::string &elseCommands) {
    std::string command;
    std::string registerValue = "$reg1";
    command += registerValue + " := " + leftValue + " - " + rightValue + ";\n";
    command += std::string("JZERO 1 %ELSE") + std::to_string(numOfIf) + "%;\n";
    command += ifCommands;
    command += std::string("JUMP %ENDIF") + std::to_string(numOfIf) + "%;\n";
    command += std::string("%ELSE") + std::to_string(numOfIf) + "%: ";
    command += elseCommands;
    command += std::string("%ENDIF") + std::to_string(numOfIf) + "%: ";
    ++numOfIf;
    return std::move(command);
}

std::string
Optimizer::generateEq(const std::string &leftValue,
                      const std::string &rightValue,
                      const std::string &ifCommands,
                      const std::string &elseCommands) {
    std::string command;
    std::string registerValue1 = "$reg1";
    std::string registerValue2 = "$reg2";
    command += registerValue1 + " := " + leftValue + " - " + rightValue + ";\n";
    command += registerValue2 + " := " + rightValue + " - " + leftValue + ";\n";
    command += std::string("JZERO 1 %NEXT") + std::to_string(numOfIf) + "%;\n";
    command += std::string("JUMP %ELSE") + std::to_string(numOfIf) + "%;\n";
    command += std::string("%NEXT") + std::to_string(numOfIf) + "%: ";
    command += std::string("JZERO 2 %IF") + std::to_string(numOfIf) + "%;\n";
    command += std::string("%ELSE") + std::to_string(numOfIf) + "%: ";
    command += elseCommands;
    command += std::string("JUMP %ENDIF") + std::to_string(numOfIf) + "%;\n";
    command += std::string("%IF") + std::to_string(numOfIf) + "%: ";
    command += ifCommands;
    command += std::string("%ENDIF") + std::to_string(numOfIf) + "%: ";
    ++numOfIf;
    return std::move(command);
}

std::string Optimizer::whileCommand(jftt::Condition condition, std::string commandsInside) {
    std::string command;
    command += std::string("%WHILE") + std::to_string(numOfWhile) + "%: ";
    command += ifCommand(condition,
                         commandsInside + "JUMP %WHILE" + std::to_string(numOfWhile) + "%;\n",
                         "SKIP;\n");
    ++numOfWhile;
    return std::move(command);
}


} // namespace optimizer