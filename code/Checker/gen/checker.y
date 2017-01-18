%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {checker}
%define parser_class_name {LexParser}
%define api.prefix {checker}

%code requires{
#include "Variable.hpp"
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
#include "Checker.hpp"
#include "CheckerScanner.hpp"
#include "debug.hpp"

#undef yylex
#define yylex scanner.checkerlex
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

%type <std::string> commands
%type <std::string> command
%type <std::string> expression
%type <std::string> condition
%type <std::string> value
%type <std::string> identifier


%%

program       : VAR vdeclarations Begin commands End { scanner.getOut() << "BEGIN\n" + $4 + "END\n"; }

vdeclarations : vdeclarations pidentifier       { driver.createVariable(jftt::VariableBuilder()
                                                                         .withName($2)
                                                                         .build()); }
               | vdeclarations pidentifier leftBracket num rightBracket{ driver.createVariable(jftt::VariableBuilder()
                                                                                                .withName($2)
                                                                                                .withNumberOfElements($4)
                                                                                                .build()); }
               |


commands    : commands command          { $$ = $1 + $2; }
             | command                  { $$ = $1; }


command     : identifier assign expression semicolon                { $$ = $1 + " := " + $3 + ";\n"; }
             | identifier assign expression                         { $$ = $1 + " := " + $3 + ";\n"; driver.warningSemicolon(); }
             | IF condition THEN commands ELSE commands ENDIF       { $$ = std::string("IF ") + $2 + " THEN\n"
                                                                            + $4 + "ELSE\n" + $6 + "ENDIF\n"; }
             | WHILE condition DO commands ENDWHILE                 { $$ = std::string("WHILE ") + $2 + " DO\n"
                                                                            + $4 + "ENDWHILE\n"; }
             | FOR pidentifier FROM value TO value DO commands ENDFOR { $$ = std::string("FOR ") + $2 +
                                                                            " FROM " + $4 + " TO " + $6 + " DO\n" +
                                                                             $8 + "ENDFOR\n";
                                                                        driver.createLocalVariable(jftt::VariableBuilder()
                                                                                              .withName($2)
                                                                                              .build()); }
             | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR { $$ = std::string("FOR ") + $2 +
                                                                            " FROM " + $4 + " DOWNTO " + $6 + " DO\n" +
                                                                             $8 + "ENDFOR\n";
                                                                            driver.createLocalVariable(jftt::VariableBuilder()
                                                                                                  .withName($2)
                                                                                                  .build()); }
             | READ identifier semicolon                        { $$ = "READ " + $2 + ";\n"; }
             | READ identifier                                  { $$ = "READ " + $2 + ";\n"; driver.warningSemicolon(); }
             | WRITE value semicolon                            { $$ = "WRITE " + $2 + ";\n"; }
             | WRITE value                                      { $$ = "WRITE " + $2 + ";\n"; driver.warningSemicolon(); }
             | SKIP semicolon                                   { $$ = "SKIP;\n"; }
             | SKIP                                             { $$ = "SKIP;\n"; driver.warningSemicolon(); }

expression  : value                         { $$ = $1; }
             | value plus value             { $$ = $1 + " + " + $3; }
             | value minus value            { $$ = $1 + " - " + $3; }
             | value mul value              { $$ = $1 + " * " + $3; }
             | value div value              { $$ = $1 + " / " + $3; }
             | value mod value              { $$ = $1 + " % " + $3; }

condition   : value equal value             { $$ = $1 + " = " + $3; }
             | value notEqual value         { $$ = $1 + " <> " + $3; }
             | value lowerThan value        { $$ = $1 + " < " + $3; }
             | value biggerThan value       { $$ = $1 + " > " + $3; }
             | value lowerOrEqThan value    { $$ = $1 + " <= " + $3; }
             | value biggerOrEqThan value   { $$ = $1 + " >= " + $3; }


value       : num                                                   { $$ = $1; }
            | identifier                                            { $$ = $1; }


identifier  : pidentifier                                           { $$ = $1; }
             | pidentifier leftBracket pidentifier rightBracket     { $$ = $1 + "[" + $3 + "]"; }
             | pidentifier leftBracket num rightBracket             { $$ = $1 + "[" + $3 + "]"; }

%%


void checker::LexParser::error( const location_type &l, const std::string &err_message )
{
   DEBUG << "Error: " << err_message << " at " << l << ", when parse " << scanner.txt <<"\n";
   //std::cerr << "Error: " << err_message << " at " << l << ", when parse " << scanner.txt <<"\n";
}