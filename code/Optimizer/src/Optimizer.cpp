//#include <Driver.hpp>
#include <inc/Expression.hpp>
#include <cstdlib>
#include "Optimizer.hpp"
#include "OptimizerScanner.hpp"
#include "debug.hpp"

namespace optimizer {

void Optimizer::run(std::istream& in) {
    auto scanner = std::make_shared<optimizer::Scanner>(in, out, Logger::out);
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

std::string Optimizer::generateEq(const std::string &leftValue,
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

std::string Optimizer::whileCommand(jftt::Condition condition, const std::string& commandsInside) {
    std::string command;
    command += std::string("%WHILE") + std::to_string(numOfWhile) + "%: ";
    command += ifCommand(condition,
                         commandsInside + "JUMP %WHILE" + std::to_string(numOfWhile) + "%;\n",
                         "SKIP;\n");
    ++numOfWhile;
    return std::move(command);
}

std::string Optimizer::ifTo(const std::string &var, const std::string &from,
                            const std::string &to, const std::string &commandsInside) {
    std::string command;
    std::string incrementVar = var + " := " + var + " + 1;\n";
    std::string repeat = "JUMP %FOR" + std::to_string(numOfFor) + "%;\n";
    command += var + " := " + from + ";\n";
    command += "%FOR" + std::to_string(numOfFor) + "%: "; //"$reg1 := " + var + " - " + to + ";\n";
    command += ifCommand(jftt::Condition{var, jftt::compare::lowerOrEqThan, to},
                         commandsInside + incrementVar + repeat,
                         "SKIP;\n");
    ++numOfFor;
    return std::move(command);
}

std::string Optimizer::ifDownTo(const std::string &var, const std::string &from, const std::string &to,
                                const std::string &commandsInside) {
    std::string command;
    std::string decrementVar = var + " := " + var + " - 1;\n";
    std::string repeat = "JUMP %FOR" + std::to_string(numOfFor) + "%;\n";
    command += var + " := " + from + ";\n";
    command += "%FOR" + std::to_string(numOfFor) + "%: "; //"$reg1 := " + var + " - " + to + ";\n";
    command += ifCommand(jftt::Condition{var, jftt::compare::biggerOrEqThan, to},
                         commandsInside + decrementVar + repeat,
                         "SKIP;\n");
    ++numOfFor;
    return std::move(command);
}

std::string Optimizer::mul(const std::string &leftVariable, const std::string &rightVariable) {
    std::string command;
    std::string leftVar = leftVariable;
    std::string rightVar = rightVariable;

    if (isVarTab(leftVar))
    {
        command += jftt::leftVar.name + " := " + leftVariable + ";\n";
        leftVar = jftt::leftVar.name;
    }
    if (isVarTab(rightVar))
    {
        command += jftt::rightVar.name + " := " + rightVariable + ";\n";
        rightVar = jftt::rightVar.name;
    }

    command += "ZERO 1;\n";
    command += "$reg2 := " + leftVar + ";\n";
    command += "$reg3 := " + rightVar + ";\n";
    command += "%MUL" + std::to_string(numOfMul) + "%: JZERO 3 %ENDMUL" + std::to_string(numOfMul) + "%;\n";
    command += "JODD 3 %ADD" + std::to_string(numOfMul) + "%;\n";
    command += "JUMP %MULSKIP" + std::to_string(numOfMul) + "%;\n";
    command += "%ADD" + std::to_string(numOfMul) + "%: " + jftt::varTemp.name + " := $reg2;\n";
    command += "ADD 1;\n";
    command += "%MULSKIP" + std::to_string(numOfMul) + "%: SHL 2;\n";
    command += "SHR 3;\n";
    command += "JUMP %MUL" + std::to_string(numOfMul) + "%;\n";
    command += std::string("%ENDMUL") + std::to_string(numOfMul) + "%: ";
    ++numOfMul;
    return std::move(command);
}

std::string Optimizer::expression(const std::string &var, const jftt::Expression &exp) {
    switch (exp.operat){
        case jftt::Operator::none:
        case jftt::Operator::plus:
        case jftt::Operator::minus:
            return var + " := " + exp.leftValue.name + exp.operat + exp.rightValue.name + ";\n";
        case jftt::Operator::mul:
            return mul(exp.leftValue.name, exp.rightValue.name) + var + " := $reg1;\n";
        case jftt::Operator::div:
            return div(exp.leftValue.name, exp.rightValue.name) + var + " := $reg1;\n";
        case jftt::Operator::modulo:
            return div(exp.leftValue.name, exp.rightValue.name) + var + " := $reg2;\n";
    }

}

std::string Optimizer::div(const std::string &leftVar, const std::string &rightVar) {
    std::string command;
    command += "ZERO 1;\n";
    command += "$reg2 := " + leftVar + ";\n";
    command += "$reg3 := " + rightVar + ";\n";

            //add:
    command += "%DIVWHILE" + std::to_string(numOfDiv) + "%: "+ jftt::varTemp.name + " := $reg3;\n";
    command += "$reg4 := " + leftVar + " - " + jftt::varTemp.name +  ";\n";
    command += "JZERO 4 %ENDDIVWHILE" + std::to_string(numOfDiv) + "%;\n";
    command += "SHL 3;\n";
    command += "JUMP %DIVWHILE" + std::to_string(numOfDiv) + "%;\n";

            //div:
    command += "%ENDDIVWHILE" + std::to_string(numOfDiv) + "%: " + jftt::varTemp.name + " := $reg3;\n";
    command += "$reg4 := " + rightVar + " - " + jftt::varTemp.name + ";\n";
    command += "JZERO 4 %DIV" + std::to_string(numOfDiv) + "%;\n";
    command += "JUMP %ENDDIV" + std::to_string(numOfDiv) + "%;\n";
    command += "%DIV" + std::to_string(numOfDiv) + "%: SHL 1;\n";
    command += jftt::varTemp.name + " := $reg3;\n";
    command += "LOAD 4;\n";
    command += "STORE 2;\n";
    command += "SUB 4;\n";

    command += "JZERO 4 %PERFORMDIV" + std::to_string(numOfDiv) + "%;\n";
    command += "JUMP %SKIPDIV" + std::to_string(numOfDiv) + "%;\n";
    command += "%PERFORMDIV" + std::to_string(numOfDiv) + "%: INC 1;\n";
    command += jftt::varTemp.name + " := $reg3;\n";
    command += "SUB 2;\n";

    command += "%SKIPDIV" + std::to_string(numOfDiv) + "%: SHR 3;\n";
    command += "JUMP %ENDDIVWHILE" + std::to_string(numOfDiv) + "%;\n";
    command += "%ENDDIV" + std::to_string(numOfDiv) + "%: ";

    ++numOfDiv;
    return std::move(command);
}

bool Optimizer::isVarTab(const std::string &var) {
    return var[var.size() - 1] == ']';
}


} // namespace optimizer