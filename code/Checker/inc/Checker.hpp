#pragma once

#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include <Variable.hpp>
#include "CheckerScanner.hpp"

namespace checker {

class Checker {
public:
    static const std::string warning;
    static const std::string missingSemicolon;

    Checker(std::ostream& out,
            std::ostream& info = std::cout,
            std::ostream& error = std::cerr)
        : out(out), info(info), error(error)
    {}

    void run(std::istream& in);
    void createVariable(const jftt::Variable &variable);

    const std::vector<std::string>& getVariables() const;

    void warningSemicolon();
    void repairSemicolon();
private:
    std::ostream& out;
    std::ostream& info;
    std::ostream& error;
    std::vector<std::string> variables;
    std::shared_ptr<Scanner> scanner;
};

} // namespace checker
