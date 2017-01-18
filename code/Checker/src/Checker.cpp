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
const std::string Checker::unrecognizedText = " - Unrecognized text";

bool Checker::run(std::istream& in) {
    scanner = std::make_shared<Scanner>(in, out, Logger::out);
    auto parser = std::make_shared<LexParser>(*scanner, *this);

    variables.push_back(jftt::varTemp.name);
    variables.push_back(jftt::leftVar.name);
    variables.push_back(jftt::rightVar.name);

    DEBUG << "starting checking\n";
    if (parser->parse()) {
        err << error + line + std::to_string(scanner->getNumOfLine() + 1) + unrecognizedText;
        return false;
    }
    DEBUG << "ended checking\n";

    for(auto var : localVariables)
    {
        variables.push_back(var.first);
    }
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

void Checker::createLocalVariable(const jftt::Variable &variable) {
    localVariables[variable.name] = 0;
}

} // namespace checker