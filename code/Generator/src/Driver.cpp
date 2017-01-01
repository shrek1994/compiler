#include <cctype>
#include <fstream>
#include <cassert>
#include <bitset>

#include "debug.hpp"
#include "Driver.hpp"

namespace jftt {

void Driver::parse( std::istream &stream )
{
   if( ! stream.good()  && stream.eof() )
   {
       return;
   }
   parse_helper( stream ); 
   return;
}


void Driver::parse_helper(std::istream &stream)
{
   scanner = std::make_shared<Scanner>(stream, std::cout);
   parser = std::make_shared<LexParser>(*scanner, *this);
   if (parser->parse())
   {
      std::cerr << "Parse failed!!\n";
   }
}

void Driver::halt()
{
    out << "HALT\n";
}

void Driver::createVariable(const Variable &variable)
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

std::string Driver::getBinaryString(long long value)
{
    auto binary = std::bitset<std::numeric_limits<decltype(value)>::digits>(value).to_string();
    auto begin = binary.find('1');
    return begin != std::string::npos ? binary.substr(begin) : "";
}

void Driver::setRegister(long long value, unsigned registerNumber)
{
    DEBUG << "setRegister(value=" << value << ", register=" << registerNumber << ")\n";
    ASSERT(registerNumber < 5);
    out << "ZERO " << registerNumber << "\n";
    if (value == 0)
        return;
    auto binary  = getBinaryString(value);
    for (int i = 0; i < binary.size() - 1; ++i)
    {
        if (binary[i] == '1')
            out << "INC " << registerNumber << "\n";
        out << "SHL " << registerNumber << "\n";
    }
    if ( binary[binary.size() - 1] == '1')
    {
        out << "INC " << registerNumber << "\n";
    }
}

void Driver::writeNumber(long long value, unsigned registerNumber)
{
    setRegister(value, registerNumber);
    out << "PUT " << registerNumber << "\n";
}

void Driver::findAndSetAction(const std::string& action, const Variable& variable)
{
    constexpr unsigned registerNumber = 2;
    setPositionInZeroRegister(variable, registerNumber);
    out << action << "\n";

}

int Driver::getPosition(const std::string &variable)
{
    int position = std::find(variables.begin(), variables.end(), variable) - variables.begin();
    DEBUG << "getPosition('" << variable << "\') = " << position << "\n";
    if ( ! (position < variables.size())) throw std::out_of_range("ERROR: variable not declared: " + variable);
    return position;
}

void Driver::loadVariable(const Variable& variable, unsigned registerNumber)
{
    findAndSetAction(std::string("LOAD ") + std::to_string(registerNumber), variable);
}

void Driver::write(const Variable &variable)
{
    constexpr unsigned registerNumber = 4;
    DEBUG << "write(" << variable << ")\n";
    if (variable.isValue)
    {
        auto value = std::atoll(variable.name.c_str());
        DEBUG << "\'" << variable.name << "\' is a number ("<< value <<")\n";
        writeNumber(value, registerNumber);
    }
    else
    {
        loadVariable(variable, registerNumber);
        out << "PUT " << registerNumber << "\n";
    }
}

void Driver::read(const Variable &variable)
{
    constexpr unsigned registerNumber = 3;
    DEBUG << "read(" << variable << ")\n";
    out << "GET " << registerNumber << "\n";
    findAndSetAction(std::string("STORE ") + std::to_string(registerNumber), variable);
}

void Driver::assignFromFirstRegisterTo(const Variable& variable)
{
    if (variable.isValue)
    {
        throw std::out_of_range("ERROR: variable is a number: " + variable.name);
    }
    constexpr unsigned registerNumber = 1;
    findAndSetAction(std::string("STORE ") + std::to_string(registerNumber), variable);
}

void Driver::saveValueToFirstRegister(const Variable& variable)
{
    constexpr unsigned registerNumber = 1;
    if (variable.isValue)
    {
        setRegister(std::atoll(variable.name.c_str()), registerNumber);
    }
    else
    {
        loadVariable(variable, registerNumber);
    }
}

void Driver::setPositionInZeroRegister(const Variable &variable, unsigned registerNumber)
{
    auto position = getPosition(variable.name);
    if ( variable.hasNameInsideTab)
    {
        auto positionOfVarInsideTab = getPosition(variable.varName);
        setRegister(positionOfVarInsideTab, 0);
        setRegister(position, registerNumber);
        out << "ADD " << registerNumber << "\n";
        out << "COPY " << registerNumber << "\n";
    }
    else
    if ( variable.isTab )
    {
        position += variable.size;
        setRegister(position, 0);
    }
    else
    {
        setRegister(position, 0);
    }
}

void Driver::saveSumToFirstRegister(const Variable &leftVar, const Variable &rightVar)
{
    constexpr unsigned registerNumber = 1;
    if ( leftVar.isValue && rightVar.isValue)
    {
        setRegister(std::atoll(leftVar.name.c_str()) + std::atoll(rightVar.name.c_str()),
                    registerNumber);
    }
    else if ( leftVar.isValue && ! rightVar.isValue)
    {
        setPositionInZeroRegister(rightVar, registerNumber);
        setRegister(std::atoll(leftVar.name.c_str()), registerNumber);
        out << "ADD " << registerNumber << "\n";

    }
    else if (  (!leftVar.isValue) && rightVar.isValue)
    {
        saveSumToFirstRegister(rightVar, leftVar);
    }
    else
    {
        loadVariable(leftVar, registerNumber);
        setPositionInZeroRegister(rightVar, registerNumber + 1);
        out << "ADD " << registerNumber << "\n";
    }
}

void Driver::saveSubToFirstRegister(const Variable &leftVar, const Variable &rightVar)
{
    constexpr unsigned registerNumber = 1;
    if ( leftVar.isValue && rightVar.isValue)
    {
        setRegister(std::atoll(leftVar.name.c_str()) - std::atoll(rightVar.name.c_str()),
                    registerNumber);
    }
    else if ( leftVar.isValue && ! rightVar.isValue)
    {
        setPositionInZeroRegister(rightVar, registerNumber);
        setRegister(std::atoll(leftVar.name.c_str()), registerNumber);
        out << "SUB " << registerNumber << "\n";

    }
    else if (  (!leftVar.isValue) && rightVar.isValue)
    {
        // Optimalization
//        auto value = std::atoll(rightVar.name.c_str());
//        if (value < 20)
//        {
//            loadVariable(leftVar, registerNumber);
//            while (value-- > 0)
//            {
//                out << "DEC " << registerNumber << "\n";
//            }
//        }
//        else
//        {

        setRegister(std::atoll(rightVar.name.c_str()), registerNumber);
        findAndSetAction(std::string("STORE ") + std::to_string(registerNumber), varTemp);
        saveSubToFirstRegister(leftVar, varTemp);
//        }
    }
    else
    {
        loadVariable(leftVar, registerNumber);
        setPositionInZeroRegister(rightVar, registerNumber + 1);
        out << "SUB " << registerNumber << "\n";
    }
}

} // namespace jftt