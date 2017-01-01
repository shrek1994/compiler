#include "Generator.hpp"
#include "Driver.hpp"


Generator::Generator(std::ostream &out)
//    : out(out)
{

}

int Generator::run(std::istream &inFile, std::ostream &outFile)
{
    jftt::Driver driver(outFile);
    driver.parse(inFile);
    return 0;
}
