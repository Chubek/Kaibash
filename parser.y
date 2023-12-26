%{

#include "tables.h"
#include "tree.h"
#include "machine.h"

#include <stdlib.h>

%}

%union
{
	struct AbsynNode* absyn_node;
	struct AbsynValue* absyn_value;
	struct AbsynValueList absyn_value_list;
}

%token <word> WORD
%token <NAME> NAME DOLLAR_NAME DOLLAR_LCURLY_NAME
%token <integer> NUM

%token SEMICOLON AND OR PIPE EXCLAIM COMMA
%token GT DOUBLE_GT LT DOUBLE_LT TRIPLE_LT 
%token LBRACK RBRACK
%token LPAREN RPAREN
%token LCURLY RCURLY
%token DOLLAR_LCURLY DOLLAR_LPAREN
%token MONAD
%token DYAD
%token NUM OP


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

for_loop: FOR NAME IN word_list DO compound_command DONE
        ;

word_list: WORD
	 | word_list WORD
         ;

case_statement: CASE word IN LPAREN pattern_list RPAREN compound_command ESAC
             ;

pattern_list: WORD
	    | pattern_list PIPE WORD
            ;

function_definition: FUNCTION NAME OPEN_CLOSE compound_command
                  ;

variable_assignment: NAME EQUALS literal
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

parameter_expansion: DOLLAR_LCURLY NAME COLON word RCURLY
		   { $$ = mK_absyn_node(ABSYN_PAREXPN, $$, $4, mk_name($2)); }
                  ;

command_substitution: DOLLAR_LPAREN compound_command RPAREN
		    { $$ = mk_absyn_node(ABSYN_COMPCMD, $$, $2, NULL); }
                   ;

variable:  DOLLAR_NAME 			{ $$ = mk_absyn_node(ABSYN_USENAME, NULL, NULL, mk_name($2));  }
	|  DOLLAR_CURLY_NAME     	{ $$ = mk_absyn_node(ABSYN_SUBSNAME, NULL, NULL, mk_name($2)); }
        ;

string: single_quoted_string	{ $$ = $1; }
      | double_quoted_string	{ $$ = $1; }
      ;

double_quoted_string: DOUBLE_QUOTE double_quoted_string_content DOUBLE_QUOTE 
		    { $$ = mk_absyn_node(ABSYN_DQSTR, NULL, NULL, mk_dqstr($2)); }
		    ;

double_quoted_string_content: SPACED_WORD	 { $$ = mk_absyn_node(ABSYN_QSTR,
			    					$$, NULL, mk_qstr($1);    }
			    | variable		 { $$ = mk_absyn_node(ABSYN_VAR,
			    					$$, $1, NULL);		  }
			    | command_substitution { $$ = mk_absyn_node(ABSYN_SUBSCMD,
			    					$$, $1, NULL);		  }
			    | parameter_expansion  { $$ = mk_absyn_node(ABSYN_SUBPARAM,
			    					$$, $1, NULL);		  }
			    ;

single_quoted_string: QUOTED_STR { $$ = mk_absyn_node(ABSYN_QSTR, NULL, NULL, mk_qstr($1)); }
		    ;


expression: NUM				 { $$ = mk_absyn_node(ABSYN_NUM, NULL, NULL, mk_num($1)); }
          | expression OP expression     { $$ = mk_absyn_node(ABSYN_EXPR, $1, $3, mk_op($2)); }
	  ;
