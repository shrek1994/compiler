#include "Checker.hpp"
#include "CheckerScanner.hpp"
#include "debug.hpp"

namespace checker {

std::stringstream Checker::run(std::istream& in) {
    std::stringstream out;
    auto scanner = std::make_shared<Scanner>(in, out);
    auto parser = std::make_shared<LexParser>(*scanner, *this);
    DEBUG << "starting checking\n";
    if (parser->parse())
        error << "Parse failed!!\n";
    DEBUG << "ended checking\n";
    return std::move(out);
}

Checker::Checker(std::ostream &info, std::ostream &error)
        : info(info),
          error(error)
{
}

} // namespace checker