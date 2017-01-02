#include "Checker.hpp"
#include "CheckerScanner.hpp"
#include "debug.hpp"

namespace checker {

const std::string Checker::warning = "Warning - line: ";
const std::string Checker::missingSemicolon = " - Missing Semicolon";

void Checker::run(std::istream& in) {
    scanner = std::make_shared<Scanner>(in, out, Logger::out);
    auto parser = std::make_shared<LexParser>(*scanner, *this);
    DEBUG << "starting checking\n";
    if (parser->parse())
        error << "Parse failed!!\n";
    DEBUG << "ended checking\n";
}

void Checker::createVariable(const jftt::Variable &variable)
{
    DEBUG << "push_back(" << variable << ");\n";

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
    info << warning << scanner->getNumOfLine() << missingSemicolon << "\n";
}

void Checker::repairSemicolon() {
    out << ";";
    std::stringstream& txt = static_cast<std::stringstream&>(out);
    auto txtStr = txt.str();
    DEBUG << "repairSemicolon()\n" << txtStr << "\n";
}

} // namespace checker