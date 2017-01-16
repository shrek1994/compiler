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
    static const std::string line;
    static const std::string warning;
    static const std::string missingSemicolon;
    static const std::string error;
    static const std::string duplicateDeclaration;
    static const std::string undeclaredVariable;
    static const std::string unrecognizedText;

    Checker(std::ostream& out,
            std::ostream& info = std::cout,
            std::ostream& err = std::cerr)
        : out(out), info(info), err(err)
    {}

    bool run(std::istream& in);
    void createVariable(const jftt::Variable &variable);

    const std::vector<std::string>& getVariables() const;

    void warningSemicolon();
    void repairSemicolon();
private:
    std::ostream& out;
    std::ostream& info;
    std::ostream& err;
    std::vector<std::string> variables;
    std::shared_ptr<Scanner> scanner;
    bool isCodeCorrect = true;

    void checkIfVaribleDontExist(const jftt::Variable &variable);
};

} // namespace checker
