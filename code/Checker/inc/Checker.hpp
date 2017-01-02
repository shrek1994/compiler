#pragma once

#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include <Variable.hpp>

namespace checker {

class Checker {
public:
    Checker(std::ostream& out,
            std::ostream& info = std::cout,
            std::ostream& error = std::cerr)
        : out(out), info(info), error(error)
    {}

    void run(std::istream& in);
    void createVariable(const jftt::Variable &variable);

    const std::vector<std::string>& getVariables() const;
private:
    std::ostream& out;
    std::ostream& info;
    std::ostream& error;
    std::vector<std::string> variables;
};

} // namespace checker
