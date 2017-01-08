#pragma once

#include <sstream>
#include <iostream>
#include <memory>
#include <inc/Condition.hpp>
#include <inc/Expression.hpp>
#include "Variable.hpp"

namespace optimizer {

class Optimizer {
public:
    Optimizer() = default;
    Optimizer(std::ostream &out);

    void run(std::istream& in);

    std::string ifCommand(jftt::Condition condition,
                          const std::string& ifCommands,
                          const std::string& elseCommands);
    std::string whileCommand(jftt::Condition condition,
                             const std::string& commandsInside);
    std::string ifTo(const std::string& var, const std::string& from,
                     const std::string& to, const std::string& commandsInside);
    std::string ifDownTo(const std::string& var, const std::string& from,
                         const std::string& to, const std::string& commandsInside);
    std::string expression(const std::string& var, const jftt::Expression& exp);

    std::ostream& getOut() const;
private:
    std::string generateBiggerThan(const std::string& leftValue,
                                   const std::string& rightValue,
                                   const std::string& ifCommands,
                                   const std::string& elseCommands);
    std::string generateEq(const std::string& leftValue,
                           const std::string& rightValue,
                           const std::string& ifCommands,
                           const std::string& elseCommands);
    std::string mul(const std::string& leftVar, const std::string& rightVar);

    std::ostream& out = std::cout;
    std::ostream& error = std::cerr;
    unsigned long numOfIf = 0;
    unsigned long numOfWhile = 0;
    unsigned long numOfFor = 0;
    unsigned long numOfMul;
};

} // namespace optimizer
