#pragma once

#include <sstream>
#include <iostream>
#include <memory>

namespace optimizer {

class Optimizer {
public:
    Optimizer() = default;

    Optimizer(std::ostream &info, std::ostream &error);

    std::stringstream run(std::istream& in);

private:
    std::ostream& info = std::cout;
    std::ostream& error = std::cerr;
};

} // namespace optimizer
