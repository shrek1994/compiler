#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "optimizer.tab.y.hpp"
#include "optimizer/location.hh"

namespace optimizer{

class Scanner : public yyFlexLexer{
public:
    Scanner(std::istream& in, std::ostream& out) : yyFlexLexer(in, out)
   {
      loc = new optimizer::LexParser::location_type();
   };

   virtual ~Scanner() {
      delete loc;
   };

   using FlexLexer::yylex;

   virtual int yylex(optimizer::LexParser::semantic_type * const lval,
                     optimizer::LexParser::location_type * location);

private:
    /* yyval ptr */
    optimizer::LexParser::semantic_type *yylval = nullptr;
    /* location ptr */
    optimizer::LexParser::location_type *loc    = nullptr;
    std::string txt;
};

} // namespace optimizer