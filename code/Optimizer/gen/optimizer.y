%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {optimizer}
%define parser_class_name {LexParser}

%code requires{
#include "Condition.hpp"
#include "Expression.hpp"

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

%type<jftt::Expression> expression
%type<std::string> command
%type<std::string> commands
%type<std::string> value
%type<std::string> identifier
%type<jftt::Condition> condition

%%

program       : Begin commands End                { driver.getOut() << "BEGIN\n" << $2 << "END\n"; }

commands    : commands command          { $$ = $1 +$2; }
             | command                  { $$ = $1; }


command     : identifier assign expression semicolon                { $$ = driver.expression($1, $3); }
             | IF condition THEN commands ELSE commands ENDIF       { $$ = driver.ifCommand($2, $4, $6); }
             | WHILE condition DO commands ENDWHILE                 { $$ = driver.whileCommand($2, $4); }
             | FOR pidentifier FROM value TO value DO commands ENDFOR     { $$ = driver.ifTo($2, $4, $6, $8); }
             | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR { $$ = driver.ifDownTo($2, $4, $6, $8); }
             | READ identifier semicolon                    { $$ = std::string("READ ") + $2 + ";\n"; }
             | WRITE value semicolon                        { $$ = std::string("WRITE ") + $2 + ";\n"; }
             | SKIP semicolon                               { $$ = "SKIP;\n"; }

expression  : value                         { $$ = {{false, $1}, jftt::Operator::none}; }
             | value plus value             { $$ = {{false, $1}, jftt::Operator::plus, {false, $3} }; }
             | value minus value            { $$ = {{false, $1}, jftt::Operator::minus, {false, $3}}; }
             | value mul value              { $$ = {{false, $1}, jftt::Operator::mul, {false, $3}}; }
             | value div value              { $$ = {{false, $1}, jftt::Operator::div, {false, $3}}; }
             | value mod value              { $$ = {{false, $1}, jftt::Operator::modulo, {false, $3}}; }

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