%{
#include "debug.hpp"
#include <memory>
#include <map>
#include <string>
namespace {
unsigned long numLine = 0;
std::map<std::string, unsigned long> places;
}
%}

%option c++
%option prefix="finder"
%option noyywrap

%%

.                       { ECHO; }
\n                      { ECHO; numLine++; }
\%[A-Z0-9]+\%           { ECHO; }
\%[A-Z0-9]+\%:          { std::string txt = yytext; txt.pop_back(); places[txt] = numLine;  }
%%
namespace flex {

std::map<std::string, unsigned long>& find(std::istream& in, std::ostream& out)
{
    numLine = 0;
    auto lexer = std::make_shared<yyFlexLexer>(in, out);
    while(lexer->yylex() != 0);
    return places;
}

}
