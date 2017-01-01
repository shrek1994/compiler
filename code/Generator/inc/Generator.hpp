#pragma once

#include <iostream>

class Generator
{
public:
    Generator(std::ostream& out = std::cout);

    int run(std::istream& inFile, std::ostream& outFile);
private:
//    std::ostream& out;
};
