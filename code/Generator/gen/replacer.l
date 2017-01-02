%{
#include "debug.hpp"
#include <memory>
#include <map>
#include <string>
namespace {
unsigned long numLine = 1;
std::map<std::string, unsigned long> places;
}
%}

%option c++
%option prefix="replacer"
%option noyywrap

%%

.                       { ECHO; }
\n                      { ECHO; numLine++; }
\%[A-Z0-9]+\%           { yyout << places.at(yytext); }
%%
namespace flex {

void replace(std::istream& in, std::ostream& out, std::map<std::string, unsigned long>& places)
{
    ::places = std::move(places);
    auto lexer = std::make_shared<yyFlexLexer>(in, out);
    while(lexer->yylex() != 0);
}

}
