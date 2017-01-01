#include "Generator.hpp"
#include "Driver.hpp"


Generator::Generator(std::ostream &out)
    : out(out)
{

}

int Generator::run(std::istream &inFile, const std::vector<std::string>& variables)
{
    jftt::Driver driver(out, variables);
    driver.parse(inFile);
    return 0;
}
