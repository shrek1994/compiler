#pragma once

#include <iostream>

namespace jftt {


class compiler {
public:
    compiler(std::ostream &info = std::cout, std::ostream &error = std::cerr);

    void run(std::istream& languageCode, std::ostream& compiledCode);

private:
    std::ostream& info;
    std::ostream& error;
};

} // namespace jftt