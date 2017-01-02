#include <cctype>
#include <fstream>
#include <bitset>
#include <Generator/inc/Finder.hpp>
#include <Generator/inc/Replacer.hpp>

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
    scanner = std::make_shared<Scanner>(stream, Logger::out);
    parser = std::make_shared<LexParser>(*scanner, *this);
    auto finder = std::make_shared<Finder>();
    auto replacer = std::make_shared<Replacer>();

    if (parser->parse())
    {
        std::cerr << "Parse failed!!\n";
        return;
    }

    DEBUG << "code: \n" << code.str() << "\n";
    std::stringstream codeStream;
    auto& position = finder->run(code, codeStream);

    DEBUG << "position.size() = " << position.size() << "\n";
    for (auto elem : position)
    {
        DEBUG << "{ " << elem.first << ", " << elem.second << " }\n";
    }

    replacer->run(codeStream, out, position);
}

void Driver::halt()
{
    code << "HALT\n";
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
    code << "ZERO " << registerNumber << "\n";
    if (value == 0)
        return;
    auto binary  = getBinaryString(value);
    for (int i = 0; i < binary.size() - 1; ++i)
    {
        if (binary[i] == '1')
            code << "INC " << registerNumber << "\n";
        code << "SHL " << registerNumber << "\n";
    }
    if ( binary[binary.size() - 1] == '1')
    {
        code << "INC " << registerNumber << "\n";
    }
}

void Driver::writeNumber(long long value, unsigned registerNumber)
{
    setRegister(value, registerNumber);
    code << "PUT " << registerNumber << "\n";
}

void Driver::findAndSetAction(const std::string& action, const Variable& variable)
{
    constexpr unsigned registerNumber = 2;
    setPositionInZeroRegister(variable, registerNumber);
    code << action << "\n";

}

int Driver::getPosition(const std::string &variable)
{
    if (variable == varTemp.name)
        return variables.size();
    auto position = std::find(variables.begin(), variables.end(), variable) - variables.begin();
    DEBUG << "getPosition('" << variable << "\') = " << position << "\n";
    if (position >= variables.size()) throw std::out_of_range("ERROR: variable not declared: " + variable);
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
        code << "PUT " << registerNumber << "\n";
    }
}

void Driver::read(const Variable &variable)
{
    constexpr unsigned registerNumber = 3;
    DEBUG << "read(" << variable << ")\n";
    code << "GET " << registerNumber << "\n";
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
        code << "ADD " << registerNumber << "\n";
        code << "COPY " << registerNumber << "\n";
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
        code << "ADD " << registerNumber << "\n";

    }
    else if (  (!leftVar.isValue) && rightVar.isValue)
    {
        saveSumToFirstRegister(rightVar, leftVar);
    }
    else
    {
        loadVariable(leftVar, registerNumber);
        setPositionInZeroRegister(rightVar, registerNumber + 1);
        code << "ADD " << registerNumber << "\n";
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
        code << "SUB " << registerNumber << "\n";

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
        code << "SUB " << registerNumber << "\n";
    }
}

void Driver::jump(const std::string &place) {
    DEBUG << "JUMP " << place << "\n";
    code << "JUMP " << place << "\n";
}

void Driver::jzero(const std::string& reg, const std::string &place) {
    DEBUG << "JZERO " << reg << " " << place << "\n";
    code << "JZERO " << reg << " " << place << "\n";
}

std::string Driver::releaseCode() {
    auto code = this->code.str();
    this->code.str("");
    return std::move(code);
}

} // namespace jftt
