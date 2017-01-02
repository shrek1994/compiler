%{
#include <string>
#include "GeneratorScanner.hpp"
#include "debug.hpp"
#undef  YY_DECL
#define YY_DECL int jftt::Scanner::yylex(jftt::LexParser::semantic_type* const lval, \
                                         jftt::LexParser::location_type* loc)
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng); txt = yytext;
using token = jftt::LexParser::token;

%}

%option debug
%option nodefault
%option yyclass="jftt::Scanner"
%option prefix="compiler"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
    yylval = lval;
%}


VAR             { if (Logger::enableDebug) ECHO; return token::VAR; }
BEGIN           { if (Logger::enableDebug) ECHO; return token::Begin; }
END             { if (Logger::enableDebug) ECHO; return token::End; }

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
SKIP            { if (Logger::enableDebug) ECHO; return token::SKIP; }

[0-9]+          { if (Logger::enableDebug) ECHO; yylval->build<std::string>(yytext); txt = yytext; return token::num; }
[_a-z]+         { if (Logger::enableDebug) ECHO; yylval->build<std::string>(yytext); return token::pidentifier; }

;               { if (Logger::enableDebug) ECHO; return token::semicolon; }
\[               { if (Logger::enableDebug) ECHO; return token::leftBracket; }
\]               { if (Logger::enableDebug) ECHO; return token::rightBracket; }
:=              { if (Logger::enableDebug) ECHO; return token::assign; }
\+              { if (Logger::enableDebug) ECHO; return token::plus; }
-               { if (Logger::enableDebug) ECHO; return token::minus; }
\*              { if (Logger::enableDebug) ECHO; return token::mul; }
\/              { if (Logger::enableDebug) ECHO; return token::div; }
\%              { if (Logger::enableDebug) ECHO; return token::mod; }


\=              { if (Logger::enableDebug) ECHO; return token::equal; }
\<\>              { if (Logger::enableDebug) ECHO; return token::notEqual; }
\<              { if (Logger::enableDebug) ECHO; return token::lowerThan; }
\>              { if (Logger::enableDebug) ECHO; return token::biggerThan; }
\<\=              { if (Logger::enableDebug) ECHO; return token::lowerOrEqThan; }
\>\=              { if (Logger::enableDebug) ECHO; return token::biggerOrEqThan; }

$reg[0-4]           { if (Logger::enableDebug) ECHO; yylval->build<unsigned>(yytext[4] - 48); return token::reg; }
\%[A-Z0-9]+\%       { if (Logger::enableDebug) ECHO; yylval->build<std::string>(yytext); return token::place; }
\%[A-Z0-9]+\%:      { if (Logger::enableDebug) ECHO; }

JUMP                { if (Logger::enableDebug) ECHO; return token::JUMP; }
JZERO               { if (Logger::enableDebug) ECHO; return token::JZERO; }

[ \t\r]+        { if (Logger::enableDebug) ECHO; }
\n              { if (Logger::enableDebug) ECHO; loc->lines(); }
%%
