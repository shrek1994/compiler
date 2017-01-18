#include <Checker.hpp>
#include <Optimizer.hpp>
#include <Generator.hpp>
#include "compiler.hpp"
#include "debug.hpp"

namespace jftt {


void compiler::run(std::istream& languageCode, std::ostream& compiledCode) {
    try {
        std::stringstream outChceker, outOprimizer;

        auto check = std::make_shared<checker::Checker>(outChceker, info, error);
        auto isCodeCorrect = check->run(languageCode);
        if (!isCodeCorrect) {
            return;
        }

        DEBUG << "code:\n" << outChceker.str() << "\n";

        optimizer::Optimizer optim(outOprimizer, check);
        optim.run(outChceker);

        DEBUG << "code:\n" << outOprimizer.str() << "\n";

        Generator gen(compiledCode);
        gen.run(outOprimizer, check->getVariables());
    }
    catch (std::exception& exception){
        error << exception.what() << "\n";
    }
}

compiler::compiler(std::ostream &info, std::ostream &error)
        : info(info),
          error(error) {}

} // namespace jftt