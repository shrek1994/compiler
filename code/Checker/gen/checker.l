%{
#include <string>
#include "CheckerScanner.hpp"
#include "debug.hpp"
#undef  YY_DECL
#define YY_DECL int checker::Scanner::yylex(checker::LexParser::semantic_type* const lval, \
                                            checker::LexParser::location_type* loc)
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng); if (Logger::enableDebug) ECHO;
using token = checker::LexParser::token;

namespace {
bool endOfVariable = false;
}
%}

%option debug
%option nodefault
%option yyclass="checker::Scanner"
%option prefix="checker"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
    yylval = lval;
%}


VAR             { endOfVariable = false; return token::VAR; }
BEGIN           { endOfVariable = true;
                  /*out << yytext;*/
                  return token::Begin; }
END             { /*out << yytext;*/ return token::End; }

IF              { /*out << yytext;*/ return token::IF; }
THEN            { /*out << yytext;*/ return token::THEN; }
ELSE            { /*out << yytext;*/ return token::ELSE; }
ENDIF           { /*out << yytext;*/ return token::ENDIF; }

WHILE           { /*out << yytext;*/ return token::WHILE; }
ENDWHILE        { /*out << yytext;*/ return token::ENDWHILE; }

FOR             { /*out << yytext;*/ return token::FOR; }
FROM            { /*out << yytext;*/ return token::FROM; }
TO              { /*out << yytext;*/ return token::TO; }
DOWNTO          { /*out << yytext;*/ return token::DOWNTO; }
DO              { /*out << yytext;*/ return token::DO; }
ENDFOR          { /*out << yytext;*/ return token::ENDFOR; }

READ            { /*out << yytext;*/ return token::READ; }
WRITE           { /*out << yytext;*/ return token::WRITE; }
SKIP            { /*out << yytext;*/ return token::SKIP; }

[0-9]+          { /*out << yytext;*/
                  yylval->build<std::string>(yytext);
                  return token::num; }
[_a-z]+         { /*out << yytext;*/
                  yylval->build<std::string>(yytext);
                  return token::pidentifier; }

;               { /*out << yytext;*/ return token::semicolon; }
\[              { /*out << yytext;*/ return token::leftBracket; }
\]              { /*out << yytext;*/ return token::rightBracket; }
:=              { /*out << yytext;*/ return token::assign; }
\+              { /*out << yytext;*/ return token::plus; }
-               { /*out << yytext;*/ return token::minus; }
\*              { /*out << yytext;*/ return token::mul; }
\/              { /*out << yytext;*/ return token::div; }
\%              { /*out << yytext;*/ return token::mod; }


\=              { /*out << yytext;*/ return token::equal; }
\<\>            { /*out << yytext;*/ return token::notEqual; }
\<              { /*out << yytext;*/ return token::lowerThan; }
\>              { /*out << yytext;*/ return token::biggerThan; }
\<\=            { /*out << yytext;*/ return token::lowerOrEqThan; }
\>\=            { /*out << yytext;*/ return token::biggerOrEqThan; }

[ \t\r]+        { /*if (endOfVariable) out << " ";*/ }
\\\n            { /*if (endOfVariable) out << " ";*/ numOfLine++; loc->lines(); }
\n              { /*if (endOfVariable) out << "\n";*/ numOfLine++; loc->lines(); }

\{              { int c;
                  while((c = yyinput()) != 0) {
                      if(c == '\n')
                          loc->lines();
                      else if(c == '}')
                          break;
                  }
                }
%%