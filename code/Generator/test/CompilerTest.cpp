#include "CompilerTest.hpp"

std::ostream& operator <<(std::ostream &out, Param param)
{
    return out << param.testName;
}
