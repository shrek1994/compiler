#include "Compiler.hpp"
#include "Driver.hpp"


Compiler::Compiler(std::ostream &out)
//    : out(out)
{

}

int Compiler::run(std::istream &inFile, std::ostream &outFile)
{
    jftt::Driver driver(outFile);
    driver.parse(inFile);
    return 0;
}
