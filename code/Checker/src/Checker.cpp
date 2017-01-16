#include "Checker.hpp"
#include "CheckerScanner.hpp"
#include "debug.hpp"

namespace checker {

const std::string Checker::line = " - line: ";
const std::string Checker::warning = "Warning";
const std::string Checker::error = "Error";
const std::string Checker::missingSemicolon = " - Missing smicolon";
const std::string Checker::duplicateDeclaration = " - Duplicate declaration";
const std::string Checker::undeclaredVariable = " - Using undeclared variable";

bool Checker::run(std::istream& in) {
    scanner = std::make_shared<Scanner>(in, out, Logger::out);
    auto parser = std::make_shared<LexParser>(*scanner, *this);
    DEBUG << "starting checking\n";
    if (parser->parse()) {
        err << "Parse failed!!\n";
        return false;
    }
    DEBUG << "ended checking\n";
    return isCodeCorrect;
}

void Checker::createVariable(const jftt::Variable &variable)
{
    DEBUG << "push_back(" << variable << ");\n";
    checkIfVaribleDontExist(variable);

    if (! variable.isTab)
    {
        variables.push_back(variable.name);
    }
    else
    {
        for (auto i = variable.size; i > 0; --i)
        {
            variables.push_back(variable.name);
        }
    }
}

const std::vector<std::string>& Checker::getVariables() const {
    return variables;
}

void Checker::warningSemicolon() {
    info << warning << line << scanner->getNumOfLine() << missingSemicolon << "\n";
}

void Checker::repairSemicolon() {
    out << ";";
    std::stringstream& txt = static_cast<std::stringstream&>(out);
    auto txtStr = txt.str();
    DEBUG << "repairSemicolon()\n" << txtStr << "\n";
}

void Checker::checkIfVaribleDontExist(const jftt::Variable &variable) {
    for (auto var : variables)
    {
        if (var == variable.name)
        {
            err << error << line << scanner->getNumOfLine() << duplicateDeclaration << ": '" << variable.name << "'\n";
            isCodeCorrect = false;
            return;
        }
    }
}

} // namespace checker