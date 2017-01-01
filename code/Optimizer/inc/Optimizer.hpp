#pragma once

#include <sstream>
#include <iostream>
#include <memory>
#include <inc/Condition.hpp>
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
                             std::string commandsInside);
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

    std::ostream& out = std::cout;
    std::ostream& error = std::cerr;
    unsigned numOfIf = 0;
    unsigned numOfWhile = 0;
};

} // namespace optimizer
