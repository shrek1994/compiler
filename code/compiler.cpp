#include <Checker.hpp>
#include <Optimizer.hpp>
#include <Generator.hpp>
#include "compiler.hpp"
#include "debug.hpp"

namespace jftt {


void compiler::run(std::istream& languageCode, std::ostream& compiledCode) {
    std::stringstream outChceker, outOprimizer;

    checker::Checker check(outChceker);
    check.run(languageCode);

    DEBUG << "code:\n" << outChceker.str() << "\n";

    optimizer::Optimizer optim(outOprimizer);
    optim.run(outChceker);

    DEBUG << "code:\n" << outOprimizer.str() << "\n";

    Generator gen(compiledCode);
    gen.run(outOprimizer, check.getVariables());
}

} // namespace jftt