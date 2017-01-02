#pragma once

#include <string>
#include <cstddef>
#include <istream>
#include <memory>
#include <vector>
#include <exception>
#include <sstream>

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
    void saveValueToFirstRegister(const Variable& variable);
    void saveSumToFirstRegister(const Variable& leftVar, const Variable& rightVar);
    void saveSubToFirstRegister(const Variable& leftVar, const Variable& rightVar);

    void jump(const std::string& place);
    void jzero(const std::string& reg, const std::string& place);

    void beginPlace(const std::string& place) { code << place; }

    std::string releaseCode();

private:
    void setRegister(long long value, unsigned registerNumber);
    std::string getBinaryString(long long value);
    void parse_helper( std::istream &stream );

    void writeNumber(long long value, unsigned registerNumber);
    void loadVariable(const Variable& variable, unsigned registerNumber);
    void findAndSetAction(const std::string& action, const Variable& variable);
    int getPosition(const std::string& variable);
    void setPositionInZeroRegister(const Variable &variable, unsigned registerNumber);

    std::shared_ptr<LexParser> parser  = nullptr;
    std::shared_ptr<Scanner> scanner = nullptr;
    std::ostream& out = std::cout;
    std::stringstream code;
    Variable varTemp = VariableBuilder().withName("TEMP123").build();
    const std::vector<std::string>& variables;
};

} // namespace jftt
