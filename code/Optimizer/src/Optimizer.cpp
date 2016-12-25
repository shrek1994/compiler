#include "Optimizer.hpp"
#include "OptimizerScanner.hpp"
#include "debug.hpp"

namespace optimizer {

std::stringstream Optimizer::run(std::istream& in) {
    std::stringstream out;
    auto scanner = std::make_shared<Scanner>(in, out);
    auto parser = std::make_shared<LexParser>(*scanner, *this);
    DEBUG << "starting optimizing\n";
    if (parser->parse())
        error << "Parse failed!!\n";
    DEBUG << "ended optimizing\n";
    return std::move(out);
}

    Optimizer::Optimizer(std::ostream &info, std::ostream &error)
        : info(info),
          error(error)
{
}

} // namespace optimizer