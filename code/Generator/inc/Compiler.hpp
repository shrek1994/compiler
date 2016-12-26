#pragma once

#include <iostream>

class Compiler
{
public:
    Compiler(std::ostream& out = std::cout);

    int run(std::istream& inFile, std::ostream& outFile);
private:
//    std::ostream& out;
};
