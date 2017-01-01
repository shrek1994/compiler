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

    void ifCommand(jftt::Condition condition,
                   const std::string& ifCommands,
                   const std::string& elseCommands);


    std::ostream &getOut() const;
private:
    std::ostream& out = std::cout;
    std::ostream& error = std::cerr;
    unsigned numOfIf = 0;
};

} // namespace optimizer
