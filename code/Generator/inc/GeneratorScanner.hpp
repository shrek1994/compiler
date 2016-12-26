#ifndef __jfttSCANNER_HPP__
#define __jfttSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "compiler.tab.y.hpp"
#include "generator/location.hh"

namespace jftt{

class Scanner : public yyFlexLexer{
public:
   
   Scanner(std::istream& in, std::ostream& out) : yyFlexLexer(in, out)
   {
      loc = new jftt::LexParser::location_type();
   };
   virtual ~Scanner() {
      delete loc;
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( jftt::LexParser::semantic_type * const lval,
              jftt::LexParser::location_type *location );
   // YY_DECL defined in jftt_lexer.l
   // Method body created by flex in jftt_lexer.yy.cc

    std::string txt;
private:
   /* yyval ptr */
   jftt::LexParser::semantic_type *yylval = nullptr;
   /* location ptr */
   jftt::LexParser::location_type *loc    = nullptr;
};

} /* end namespace jftt */

#endif /* END __jfttSCANNER_HPP__ */
