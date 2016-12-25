%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {checker}
%define parser_class_name {LexParser}

%code requires{
   namespace checker {
      class Checker;
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
%parse-param { Checker  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   /* include for all driver functions */
   #include "Checker.hpp"
   #include "CheckerScanner.hpp"

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


%%

program       : VAR vdeclarations Begin commands End

vdeclarations : vdeclarations pidentifier
               | vdeclarations pidentifier leftBracket num rightBracket
               |


commands    : commands command
             | command


command     : identifier assign expression semicolon
             | IF condition THEN commands ELSE commands ENDIF
             | WHILE condition DO commands ENDWHILE
             | FOR pidentifier FROM value TO value DO commands ENDFOR
             | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR
             | READ identifier semicolon
             | WRITE value semicolon
             | SKIP semicolon

expression  : value
             | value plus value
             | value minus value
             | value mul value
             | value div value
             | value mod value

condition   : value equal value
             | value notEqual value
             | value lowerThan value
             | value biggerThan value
             | value lowerOrEqThan value
             | value biggerOrEqThan value


value       : num
            | identifier


identifier  : pidentifier
             | pidentifier leftBracket pidentifier rightBracket
             | pidentifier leftBracket num rightBracket

%%


void checker::LexParser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}