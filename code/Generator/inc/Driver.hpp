#pragma once

#include <string>
#include <cstddef>
#include <istream>
#include <memory>
#include <vector>
#include <exception>
#include <sstream>
#include <inc/Expression.hpp>

#include "GeneratorScanner.hpp"
#include "compiler.tab.y.hpp"
#include "Variable.hpp"

namespace jftt{

class Driver{
public:
    Driver(std::ostream& out, const std::vector<std::string>& variables)
       : out(out), variables(variables)
    {}

    virtual ~Driver() = default;

    void parse( std::istream &iss );

    void halt();
    void write(const Variable& variableName);
    void read(const Variable& variableName);

    void assignFromFirstRegisterTo(const Variable& variable);
    void saveValueTo(const Variable &variable, unsigned registerNumber = 1);
    void saveSumTo(const Variable &leftVar, const Variable &rightVar, unsigned registerNumber = 1);
    void saveSubToFirstRegister(const Variable& leftVar, const Variable& rightVar, unsigned registerNumber = 1);
    void saveRegisterToVar(const Variable& var, unsigned registerNumber);

    void addCommand(const std::string& reg);
    void beginPlace(const std::string& place) { code << place; }

    std::string releaseCode();
    void saveExpression(const jftt::Expression& expression, unsigned registerNumber = 1);

private:
    void setRegister(long long value, unsigned registerNumber);
    std::string getBinaryString(long long value);
    void parse_helper( std::istream &stream );

    void writeNumber(long long value, unsigned registerNumber);
    void loadVariable(const Variable& variable, unsigned registerNumber);
    void findAndSetAction(const std::string& action, const Variable& variable);
    long long getPosition(const std::string& variable);
    void setPositionInZeroRegister(const Variable &variable, unsigned registerNumber);

    std::shared_ptr<LexParser> parser  = nullptr;
    std::shared_ptr<Scanner> scanner = nullptr;
    std::ostream& out = std::cout;
    std::stringstream code;
    const std::vector<std::string>& variables;
};

} // namespace jftt
