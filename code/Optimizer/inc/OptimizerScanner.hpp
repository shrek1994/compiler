#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "optimizer.tab.y.hpp"
#include "optimizer/location.hh"

namespace optimizer {

#include "Scanner.hpp"

} // namespace optimizer