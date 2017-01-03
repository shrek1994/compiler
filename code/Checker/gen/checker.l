%{
#include <string>
#include "CheckerScanner.hpp"
#include "debug.hpp"
#undef  YY_DECL
#define YY_DECL int checker::Scanner::yylex(checker::LexParser::semantic_type* const lval, \
                                            checker::LexParser::location_type* loc)
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng); if (Logger::enableDebug) ECHO; txt = yytext;
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
                  return token::Begin; }
END             { return token::End; }

IF              { return token::IF; }
THEN            { return token::THEN; }
ELSE            { return token::ELSE; }
ENDIF           { return token::ENDIF; }

WHILE           { return token::WHILE; }
ENDWHILE        { return token::ENDWHILE; }

FOR             { return token::FOR; }
FROM            { return token::FROM; }
TO              { return token::TO; }
DOWNTO          { return token::DOWNTO; }
DO              { return token::DO; }
ENDFOR          { return token::ENDFOR; }

READ            { return token::READ; }
WRITE           { return token::WRITE; }
SKIP            { return token::SKIP; }

[0-9]+          { yylval->build<std::string>(yytext);
                  return token::num; }
[_a-z]+         { yylval->build<std::string>(yytext);
                  return token::pidentifier; }

;               { return token::semicolon; }
\[              { return token::leftBracket; }
\]              { return token::rightBracket; }
:=              { return token::assign; }
\+              { return token::plus; }
-               { return token::minus; }
\*              { return token::mul; }
\/              { return token::div; }
\%              { return token::mod; }


\=              { return token::equal; }
\<\>            { return token::notEqual; }
\<              { return token::lowerThan; }
\>              { return token::biggerThan; }
\<\=            { return token::lowerOrEqThan; }
\>\=            { return token::biggerOrEqThan; }

[ \t\r]+        { }
\\\n            { numOfLine++; loc->lines(); }
\n              { numOfLine++; loc->lines(); }

\{              { int c;
                  while((c = yyinput()) != 0) {
                      if(c == '\n')
                          loc->lines();
                      else if(c == '}')
                          break;
                  }
                }
%%