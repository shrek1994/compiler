%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {jftt}
%define parser_class_name {LexParser}
%define api.prefix {compiler}

%code requires{
   namespace jftt {
      class Driver;
      class Scanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "Variable.hpp"
}

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "Driver.hpp"

#undef yylex
#define yylex scanner.compilerlex
}

%define api.value.type variant
%define parse.assert

%token VAR
%token Begin
%token End

%token IF
%token THEN
%token ELSE
%token ENDIF

%token WHILE
%token ENDWHILE

%token FOR
%token FROM
%token TO
%token DOWNTO
%token DO
%token ENDFOR

%token READ
%token WRITE
%token SKIP

%token <std::string> num
%token <std::string> pidentifier

%token semicolon

%token leftBracket
%token rightBracket
%token assign
%token plus
%token minus
%token mul
%token div
%token mod

%token equal
%token notEqual
%token lowerThan
%token biggerThan
%token lowerOrEqThan
%token biggerOrEqThan

%token reg
%token beginPlace
%token JUMP
%token JZERO
%token <std::string> place

%locations

%type<Variable> value
%type<unsigned> reg
%type<Variable> identifier

%%

program       : Begin commands End { driver.halt();}


commands    : commands command
             | command


command     : identifier assign expression semicolon            { driver.assignFromFirstRegisterTo($1); }
             | reg assign expression semicolon
             | READ identifier semicolon                          { driver.read($2); }
             | WRITE value semicolon                              { driver.write($2); }
             | SKIP semicolon                                     {}
             | JUMP place semicolon                               { driver.jump($2); }
             | JZERO num place semicolon                          { driver.jzero($2, $3); }
             | beginPlace

expression  : value                             { driver.saveValueToFirstRegister($1); }
             | value plus value                 { driver.saveSumToFirstRegister($1, $3); }
             | value minus value                { driver.saveSubToFirstRegister($1, $3); }
             | value mul value
             | value div value
             | value mod value

condition   : value equal value
             | value notEqual value
             | value lowerThan value
             | value biggerThan value
             | value lowerOrEqThan value
             | value biggerOrEqThan value


value       : num               { $$ = VariableBuilder()
                                        .withName($1)
                                        .isValue()
                                        .build(); }
            | identifier        { $$ = $1; }


identifier  : pidentifier                                   { $$ = VariableBuilder()
                                                                    .withName($1)
                                                                    .build(); }
             | pidentifier leftBracket pidentifier rightBracket { $$ = VariableBuilder()
                                                                        .withName($1)
                                                                        .withNameInsideTab($3)
                                                                        .build(); }
             | pidentifier leftBracket num rightBracket       { $$ = VariableBuilder()
                                                                        .withName($1)
                                                                        .withNumberOfElements($3)
                                                                        .build(); }

%%


void jftt::LexParser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << ", when parse " << scanner.txt <<"\n";
}
