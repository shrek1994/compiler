#pragma once

#include <iostream>

namespace jftt {


class compiler {
public:
    void run(std::istream& languageCode, std::ostream& compiledCode);
};

} // namespace jftt