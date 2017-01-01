%{
#include <string>
#include "CheckerScanner.hpp"
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


VAR             { return token::VAR; }
BEGIN           { ECHO; endOfVariable = true; return token::Begin; }
END             { ECHO; return token::End; }

IF              { ECHO; return token::IF; }
THEN            { ECHO; return token::THEN; }
ELSE            { ECHO; return token::ELSE; }
ENDIF           { ECHO; return token::ENDIF; }

WHILE           { ECHO; return token::WHILE; }
ENDWHILE        { ECHO; return token::ENDWHILE; }

FOR             { ECHO; return token::FOR; }
FROM            { ECHO; return token::FROM; }
TO              { ECHO; return token::TO; }
DOWNTO          { ECHO; return token::DOWNTO; }
DO              { ECHO; return token::DO; }
ENDFOR          { ECHO; return token::ENDFOR; }

READ            { ECHO; return token::READ; }
WRITE           { ECHO; return token::WRITE; }
SKIP            { ECHO; return token::SKIP; }

[0-9]+          { if (endOfVariable) ECHO; yylval->build<std::string>(yytext); txt = yytext; return token::num; }
[_a-z]+         { if (endOfVariable) ECHO; yylval->build<std::string>(yytext); return token::pidentifier; }

;               { ECHO; return token::semicolon; }
\[               { if (endOfVariable) ECHO; return token::leftBracket; }
\]               { if (endOfVariable) ECHO; return token::rightBracket; }
:=              { ECHO; return token::assign; }
\+              { ECHO; return token::plus; }
-               { ECHO; return token::minus; }
\*              { ECHO; return token::mul; }
\/              { ECHO; return token::div; }
\%              { ECHO; return token::mod; }


\=              { ECHO; return token::equal; }
\<\>            { ECHO; return token::notEqual; }
\<              { ECHO; return token::lowerThan; }
\>              { ECHO; return token::biggerThan; }
\<\=            { ECHO; return token::lowerOrEqThan; }
\>\=            { ECHO; return token::biggerOrEqThan; }

[[:space:]]+    { if (endOfVariable) ECHO;  loc->lines();}
%%