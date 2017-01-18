%{
#include <string>
#include "GeneratorScanner.hpp"
#include "debug.hpp"
#undef  YY_DECL
#define YY_DECL int jftt::Scanner::yylex(jftt::LexParser::semantic_type* const lval, \
                                         jftt::LexParser::location_type* loc)
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng); txt = yytext; if (Logger::enableDebug) ECHO;
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


VAR             { return token::VAR; }
BEGIN           { return token::Begin; }
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

[0-9]+          { yylval->build<std::string>(yytext); return token::num; }
[_a-z]+         { yylval->build<std::string>(yytext); return token::pidentifier; }

;               { return token::semicolon; }
\[               { return token::leftBracket; }
\]               { return token::rightBracket; }
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
\<\=              { return token::lowerOrEqThan; }
\>\=              { return token::biggerOrEqThan; }

TEMP123             { yylval->build<std::string>(yytext); return token::pidentifier; }
LEFT123             { yylval->build<std::string>(yytext); return token::pidentifier; }
RIGHT123             { yylval->build<std::string>(yytext); return token::pidentifier; }
$reg[0-4]           { yylval->build<unsigned>(yytext[4] - 48); return token::reg; }
\%[A-Z0-9]+\%       { yylval->build<std::string>(yytext); return token::place; }
\%[A-Z0-9]+\%:      { yylval->build<std::string>(yytext); return token::beginPlace;}

JUMP                { return token::JUMP; }
JZERO               { return token::JZERO; }
ZERO                { return token::ZERO; }
JODD                { return token::JODD; }
ADD                { return token::ADD; }
SHL                { return token::SHL; }
SHR                { return token::SHR; }
LOAD                { return token::LOAD; }
STORE                { return token::STORE; }
INC                { return token::INC; }
SUB                { return token::SUB; }

[ \t\r]+        { }
\n              { numOfLine++; loc->lines(); }
%%
