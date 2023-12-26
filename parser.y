%{

#include "tables.h"
#include "machine.h"

%}

%union
{
	struct Word* word;
	struct Name* name;
	struct Pattern* pattern;
	struct StackValue* stack_value;
	struct intmax_t integer;
}

%token <word> WORD
%token <name> NAME
%token <integer> NUM

%token SEMICOLON AND OR PIPE GT DOUBLE_GT LT DOUBLE_LT TRIPLE_LT WORD WORD WORD LBRACK EXCLAIM LPAREN DASH_C DASH_D DASH_E DASH_F DASH_G DASH_L DASH_P DASH_R DASH_S DASH_U DASH_W DASH_X DASH_Z DASH_SMALL_A DASH_SMALL_B DASH_SMALL_C DASH_SMALL_D DASH_SMALL_E DASH_SMALL_F DASH_SMALL_G DASH_SMALL_H DASH_SMALL_K DASH_SMALL_P DASH_SMALL_R DASH_SMALL_S DASH_SMALL_T DASH_SMALL_U DASH_SMALL_W DASH_SMALL_X DASH_SMALL_Z IF WORD NUM COMMA DOLLAR DOLLAR_LBRACE

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
         | test_expression "-a" test_expression
         | test_expression "-o" test_expression
         | LPAREN test_expression RPAREN
         ;

unary_test: unary_operator WORD
          | unary_operator LBRACK WORD RBRACK
          | unary_operator test_expression
          ;

unary_operator: DASH_B 
	      | DASH_C 
	      | DASH_D 
	      | DASH_E 
	      | DASH_F 
	      | DASH_G 
	      | DASH_L 
	      | DASH_P 
	      | DASH_R 
	      | DASH_S 
	      | DASH_U 
	      | DASH_W 
	      | DASH_X 
	      | DASH_Z 
	      | DASH_SMALL_A 
	      | DASH_SMALL_B 
	      | DASH_SMALL_C 
	      | DASH_SMALL_D 
	      | DASH_SMALL_E 
	      | DASH_SMALL_F 
	      | DASH_SMALL_G 
	      | DASH_SMALL_H 
	      | DASH_SMALL_K 
	      | DASH_SMALL_P 
	      | DASH_SMALL_R 
	      | DASH_SMALL_S 
	      | DASH_SMALL_T 
	      | DASH_SMALL_U 
	      | DASH_SMALL_W 
	      | DASH_SMALL_X 
	      | DASH_SMALL_Z
              ;

test_operator: DASH_NE | DASH_EQ | DASH_NE | DASH_LT | DASH_LE | DASH_GT | DASH_GE
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

compound_command: LBRACE command_list RLBRACE
               ;

command_list : command
	     | command COMMA command_list
	     ;

parameter_expansion: DOLLAR_LBRACE name COLON word  RBRACE
                  ;

command_substitution: DOLLAR_LPAREN compound_command RPAREN
                   ;

variable:  DOLLAR name 
	|  DOLLAR_LBRACE name RBRACE
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
