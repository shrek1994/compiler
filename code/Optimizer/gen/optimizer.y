%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {optimizer}
%define parser_class_name {LexParser}

%code requires{
#include "Condition.hpp"

namespace optimizer {
    class Optimizer;
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

}

%parse-param { Scanner  &scanner  }
%parse-param { Optimizer  &driver  }

%code{
#include <iostream>
#include <cstdlib>
#include <fstream>

/* include for all driver functions */
#include "Optimizer.hpp"
#include "OptimizerScanner.hpp"

#undef yylex
#define yylex scanner.yylex
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

%locations

%type<std::string> expression
%type<std::string> command
%type<std::string> commands
%type<std::string> value
%type<std::string> identifier
%type<jftt::Condition> condition

%%

program       : Begin commands End                { driver.getOut() << "BEGIN\n" << $2 << "END\n"; }

commands    : commands command          { $$ = $1 +$2; }
             | command                  { $$ = $1; }


command     : identifier assign expression semicolon                { $$ = $1 + " := " + $3 + ";\n"; }
             | IF condition THEN commands ELSE commands ENDIF       { $$ = driver.ifCommand($2, $4, $6); }
             | WHILE condition DO commands ENDWHILE
             | FOR pidentifier FROM value TO value DO commands ENDFOR
             | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR
             | READ identifier semicolon
             | WRITE value semicolon                        { $$ = std::string("WRITE ") + $2 + ";\n"; }
             | SKIP semicolon                               { $$ = "SKIP;\n"; }

expression  : value                         { $$ = $1; }
             | value plus value             { $$ = $1 + " + " + $3; }
             | value minus value            { $$ = $1 + " - " + $3; }
             | value mul value              { $$ = $1 + " * " + $3; }
             | value div value              { $$ = $1 + " / " + $3; }
             | value mod value              { $$ = $1 + " % " + $3; }

condition   : value equal value             { $$ = jftt::Condition{$1,
                                                                   jftt::compare::eq,
                                                                   $3}; }
             | value notEqual value         { $$ = jftt::Condition{$1,
                                                                   jftt::compare::notEq,
                                                                   $3}; }
             | value lowerThan value        { $$ = jftt::Condition{$1,
                                                                   jftt::compare::lowerThan,
                                                                   $3}; }
             | value biggerThan value       { $$ = jftt::Condition{$1,
                                                                   jftt::compare::biggerThan,
                                                                   $3}; }
             | value lowerOrEqThan value    { $$ = jftt::Condition{$1,
                                                                   jftt::compare::lowerOrEqThan,
                                                                   $3}; }
             | value biggerOrEqThan value   { $$ = jftt::Condition{$1,
                                                                   jftt::compare::biggerOrEqThan,
                                                                   $3}; }


value       : num                                                   { $$ = $1; }
            | identifier                                            { $$ = $1; }


identifier  : pidentifier                                           { $$ = $1; }
             | pidentifier leftBracket pidentifier rightBracket     { $$ = $1 + "[" + $3 + "]"; }
             | pidentifier leftBracket num rightBracket             { $$ = $1 + "[" + $3 + "]"; }

%%


void optimizer::LexParser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}