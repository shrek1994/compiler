#pragma once

#include <iostream>
#include <vector>

class Generator
{
public:
    Generator(std::ostream& out = std::cout);

    int run(std::istream& inFile, const std::vector<std::string>& variables);
private:
    std::ostream& out;
};
