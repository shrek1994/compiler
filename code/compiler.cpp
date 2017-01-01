#include <Checker.hpp>
#include <Optimizer.hpp>
#include <Generator.hpp>
#include "compiler.hpp"

namespace jftt {


void compiler::run(std::istream& languageCode, std::ostream& compiledCode) {
    std::stringstream outChceker, outOprimizer;

    checker::Checker check(outChceker);
    check.run(languageCode);

    optimizer::Optimizer optim(outOprimizer);
    optim.run(outChceker);

    Generator gen(compiledCode);
    gen.run(outOprimizer, check.getVariables());
}

} // namespace jftt