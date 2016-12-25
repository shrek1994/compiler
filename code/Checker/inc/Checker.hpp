#pragma once

#include <sstream>
#include <iostream>
#include <memory>

namespace checker {

class Checker {
public:
    Checker() = default;

    Checker(std::ostream &info, std::ostream &error);

    std::stringstream run(std::istream& in);

private:
    std::ostream& info = std::cout;
    std::ostream& error = std::cerr;
};

} // namespace checker
