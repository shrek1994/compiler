%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "GeneratorScanner.hpp"
#undef  YY_DECL
#define YY_DECL int jftt::Scanner::yylex( jftt::LexParser::semantic_type * const lval, jftt::LexParser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = jftt::LexParser::token;

/* define yyterminate as this instead of NULL */
//#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

namespace {
bool enableDebug = false;
//bool enableDebug = true;
}
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


VAR             { if (enableDebug) ECHO; return token::VAR; }
BEGIN           { if (enableDebug) ECHO; return token::Begin; }
END             { if (enableDebug) ECHO; return token::End; }

IF              { if (enableDebug) ECHO; return token::IF; }
THEN            { if (enableDebug) ECHO; return token::THEN; }
ELSE            { if (enableDebug) ECHO; return token::ELSE; }
ENDIF           { if (enableDebug) ECHO; return token::ENDIF; }

WHILE           { if (enableDebug) ECHO; return token::WHILE; }
ENDWHILE        { if (enableDebug) ECHO; return token::ENDWHILE; }

FOR             { if (enableDebug) ECHO; return token::FOR; }
FROM            { if (enableDebug) ECHO; return token::FROM; }
TO              { if (enableDebug) ECHO; return token::TO; }
DOWNTO          { if (enableDebug) ECHO; return token::DOWNTO; }
DO              { if (enableDebug) ECHO; return token::DO; }
ENDFOR          { if (enableDebug) ECHO; return token::ENDFOR; }

READ            { if (enableDebug) ECHO; return token::READ; }
WRITE           { if (enableDebug) ECHO; return token::WRITE; }
SKIP            { if (enableDebug) ECHO; return token::SKIP; }

[0-9]+          { if (enableDebug) ECHO; yylval->build<std::string>(yytext); txt = yytext; return token::num; }
[_a-z]+         { if (enableDebug) ECHO; yylval->build<std::string>(yytext); return token::pidentifier; }

;               { if (enableDebug) ECHO; return token::semicolon; }
\[               { if (enableDebug) ECHO; return token::leftBracket; }
\]               { if (enableDebug) ECHO; return token::rightBracket; }
:=              { if (enableDebug) ECHO; return token::assign; }
\+              { if (enableDebug) ECHO; return token::plus; }
-               { if (enableDebug) ECHO; return token::minus; }
\*              { if (enableDebug) ECHO; return token::mul; }
\/              { if (enableDebug) ECHO; return token::div; }
\%              { if (enableDebug) ECHO; return token::mod; }


\=              { if (enableDebug) ECHO; return token::equal; }
\<\>              { if (enableDebug) ECHO; return token::notEqual; }
\<              { if (enableDebug) ECHO; return token::lowerThan; }
\>              { if (enableDebug) ECHO; return token::biggerThan; }
\<\=              { if (enableDebug) ECHO; return token::lowerOrEqThan; }
\>\=              { if (enableDebug) ECHO; return token::biggerOrEqThan; }


[[:space:]]+     { if (enableDebug) ECHO;  loc->lines();}
%%
