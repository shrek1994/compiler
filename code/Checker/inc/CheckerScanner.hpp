#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "checker.tab.y.hpp"
#include "checker/location.hh"

namespace checker{

#include "Scanner.hpp"

} // namespace checker