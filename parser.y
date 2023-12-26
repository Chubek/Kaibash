%{

#include "tables.h"
#include "tree.h"
#include "machine.h"

%}

%union
{
	struct AbsynNode* absyn_node;
	struct AbsynValue* absyn_value;
	struct AbsynValueList absyn_value_list;
}

%token <word> WORD
%token <name> NAME
%token <integer> NUM

%token SEMICOLON AND OR PIPE EXCLAIM COMMA
%token GT DOUBLE_GT LT DOUBLE_LT TRIPLE_LT 
%token LBRACK RBRACK
%token LPAREN RPAREN
%token LCURLY RCURLY
%token DOLLAR_LCURLY DOLLAR_LPAREN
%token MONAD
%token DYAD
%token NUM DOUBLE_LPAREN DOUBLE_RPAREN PLUS MINUS TIMES DIVIDE MODULO POWERS 

%%

program: command_list
        | command_list NEWLINE
        ;


command_list: command
            | command_list separator command
            ;

separator: SEMICOLON
        | AND
        | OR
        | PIPE
        ;

command: simple_command
       | pipeline
       | if_statement
       | while_loop
       | for_loop
       | case_statement
       | function_definition
       | variable_assignment
       | comment
       | redirection
       | compound_command
       | test_command
       | expr_substitution
       ;

expr_substitution: DOUBLE_LPAREN expression DOUBLE_RPAREN
	;

simple_command: word_list
             | word_list redirection
             ;

redirection: GT WORD
	   | DOUBLE_GT WORD
 	   | LT WORD
	   | DOUBLE_LT WORD
	   | TRIPLE_LT WORD
	   | duplicate WORD
           ;

duplciate: WORD AND_GT WORD
	 | WORD AND_LT WORD
	 ;

test_command: LBRACK test_expression RBRACK
            ;

test_expression: test_term
              | test_term test_operator test_term
              ;

test_term: unary_test
         | LBRACK test_expression RBRACK
         | EXCLAIM test_expression
         | test_expression DYAD test_expression
         | LPAREN test_expression RPAREN
         ;

unary_test: MONAD WORD
          | MONAD LBRACK WORD RBRACK
          | MONAD test_expression
          ;

if_statement: IF compound_command THEN compound_command ELSE compound_command FI
              IF compound_command THEN compound_command FI
            ;

while_loop: WHILE compound_command DO compound_command DONE
          ;

for_loop: FOR name IN word_list DO compound_command DONE
        ;

word_list: WORD
	 | word_list WORD
         ;

case_statement: CASE word IN LPAREN pattern_list RPAREN compound_command ESAC
             ;

pattern_list: WORD
	    | pattern_list PIPE WORD
            ;

function_definition: FUNCTION name OPEN_CLOSE compound_command
                  ;

variable_assignment: name EQUALS literal
                  ;

literal: WORD
       | NUM
       | string
       ;

compound_command: LCURLY command_list RLCURLY
               ;

command_list : command
	     | command COMMA command_list
	     ;

parameter_expansion: DOLLAR_LCURLY name COLON word  RCURLY
                  ;

command_substitution: DOLLAR_LPAREN compound_command RPAREN
                   ;

variable:  DOLLAR name 
	|  DOLLAR_LCURLY name RCURLY
        ;

string: single_quoted_string
      | double_quoted_string
      ;

double_quoted_string: DOUBLE_QUOTE double_quoted_string_content DOUBLE_QUOTE
		    ;

double_quoted_string_content: word_list
			    | compound_command
			    | variable
			    | command_substitution
			    | parameter_expansion
			    ;

single_quoted_string: SINGLE_QUOTE word_list SINGLE_QUOTE
		    ;


expression: NUM
	  | expression PLUS expression
	  | expression MINUS expression
	  | expression TIMES expression
	  | expression DIVIDES expression
	  | expression MODULO expression
	  | expression POWERS expression
	  | expression DOUBLE_LT expression
	  | expression DOUBLE_GT expression
	  ;
