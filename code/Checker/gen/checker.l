%{
#include <string>
#include "CheckerScanner.hpp"
#include "debug.hpp"
#undef  YY_DECL
#define YY_DECL int checker::Scanner::yylex(checker::LexParser::semantic_type* const lval, \
                                            checker::LexParser::location_type* loc)
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
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


VAR             { if (Logger::enableDebug) ECHO; return token::VAR; }
BEGIN           { if (Logger::enableDebug) ECHO;
                  endOfVariable = true;
                  out << yytext;
                  return token::Begin; }
END             { if (Logger::enableDebug) ECHO; out << yytext; return token::End; }

IF              { if (Logger::enableDebug) ECHO; return token::IF; }
THEN            { if (Logger::enableDebug) ECHO; return token::THEN; }
ELSE            { if (Logger::enableDebug) ECHO; return token::ELSE; }
ENDIF           { if (Logger::enableDebug) ECHO; return token::ENDIF; }

WHILE           { if (Logger::enableDebug) ECHO; return token::WHILE; }
ENDWHILE        { if (Logger::enableDebug) ECHO; return token::ENDWHILE; }

FOR             { if (Logger::enableDebug) ECHO; return token::FOR; }
FROM            { if (Logger::enableDebug) ECHO; return token::FROM; }
TO              { if (Logger::enableDebug) ECHO; return token::TO; }
DOWNTO          { if (Logger::enableDebug) ECHO; return token::DOWNTO; }
DO              { if (Logger::enableDebug) ECHO; return token::DO; }
ENDFOR          { if (Logger::enableDebug) ECHO; return token::ENDFOR; }

READ            { if (Logger::enableDebug) ECHO; return token::READ; }
WRITE           { if (Logger::enableDebug) ECHO; return token::WRITE; }
SKIP            { if (Logger::enableDebug) ECHO; out << yytext; return token::SKIP; }

[0-9]+          { if (Logger::enableDebug) ECHO;
                  if (endOfVariable) out << yytext;
                  yylval->build<std::string>(yytext);
                  txt = yytext;
                  return token::num; }
[_a-z]+         { if (Logger::enableDebug) ECHO;
                  if (endOfVariable) out << yytext;
                  yylval->build<std::string>(yytext);
                  return token::pidentifier; }

;               { if (Logger::enableDebug) ECHO; out << yytext; return token::semicolon; }
\[              { if (Logger::enableDebug) ECHO; return token::leftBracket; }
\]              { if (Logger::enableDebug) ECHO; return token::rightBracket; }
:=              { if (Logger::enableDebug) ECHO; return token::assign; }
\+              { if (Logger::enableDebug) ECHO; return token::plus; }
-               { if (Logger::enableDebug) ECHO; return token::minus; }
\*              { if (Logger::enableDebug) ECHO; return token::mul; }
\/              { if (Logger::enableDebug) ECHO; return token::div; }
\%              { if (Logger::enableDebug) ECHO; return token::mod; }


\=              { if (Logger::enableDebug) ECHO; return token::equal; }
\<\>            { if (Logger::enableDebug) ECHO; return token::notEqual; }
\<              { if (Logger::enableDebug) ECHO; return token::lowerThan; }
\>              { if (Logger::enableDebug) ECHO; return token::biggerThan; }
\<\=            { if (Logger::enableDebug) ECHO; return token::lowerOrEqThan; }
\>\=            { if (Logger::enableDebug) ECHO; return token::biggerOrEqThan; }

[ \t\r]+        { if (Logger::enableDebug) ECHO; if (endOfVariable) out << " "; }
\\\n            { if (Logger::enableDebug) ECHO; if (endOfVariable) out << " "; loc->lines(); }
\n              { if (Logger::enableDebug) ECHO; if (endOfVariable) out << "\n"; loc->lines(); }
%%