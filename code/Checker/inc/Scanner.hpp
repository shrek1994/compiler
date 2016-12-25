#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "checker.tab.y.hpp"
#include "location.hh"

namespace checker{

class Scanner : public yyFlexLexer{
public:
   Scanner(std::istream& in, std::ostream& out) : yyFlexLexer(in, out)
   {
      loc = new checker::LexParser::location_type();
   };

   virtual ~Scanner() {
      delete loc;
   };

   using FlexLexer::yylex;

   virtual int yylex(checker::LexParser::semantic_type * const lval,
                     checker::LexParser::location_type * location);

private:
    /* yyval ptr */
    checker::LexParser::semantic_type *yylval = nullptr;
    /* location ptr */
    checker::LexParser::location_type *loc    = nullptr;
    std::string txt;
};

} // namespace checker