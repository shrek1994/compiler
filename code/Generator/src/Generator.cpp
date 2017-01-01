#include "Generator.hpp"
#include "Driver.hpp"


Generator::Generator(std::ostream &out)
    : out(out)
{

}

int Generator::run(std::istream &inFile)
{
    jftt::Driver driver(out);
    driver.parse(inFile);
    return 0;
}
