#pragma once
#include <string>

namespace jftt
{

struct Variable {
    bool isValue = false;
    std::string name = "";
    bool isTab = false;
    unsigned size = 0;
    bool hasNameInsideTab = false;
    std::string varName = "";
};

inline std::ostream& operator << (std::ostream& out, const Variable& variable)
{
    return out << variable.name
               << ( variable.isTab ? std::string("[") + std::to_string(variable.size) + "]" : "")
               << ( variable.hasNameInsideTab ? std::string("[") + variable.varName + "]" : "");
}

class VariableBuilder
{
public:
    auto& isValue()
    {
        var.isValue = true;
        return *this;
    }

    auto& withName(std::string name)
    {
        var.name = name;
        return *this;
    }

    auto& withNumberOfElements(std::string number)
    {
        var.isTab = true;
        var.size = std::atoi(number.c_str());
        return *this;
    }

    auto& withNameInsideTab(std::string name)
    {
        var.hasNameInsideTab = true;
        var.varName = name;
        return *this;
    }

    auto build()
    {
        return var;
    }
private:
    Variable var;
};


} //namespace jftt
