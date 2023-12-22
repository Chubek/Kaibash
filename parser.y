%{
#include "tables.h"
#include "kaibash.h"


%}

%token SEMICOLON
%token PIPE
%token AMPERSAND
%token IF
%token THEN
%token ELSE
%token FI
%token WHILE
%token UNTIL
%token DO
%token DONE
%token FOR
%token IN
%token CASE
%token ESAC
%token OPEN_CLOSE
%token ATSIGN_LCURLY
%token DOLLAR_LCURLY
%token DOLLAR_LPAREN
%token DOUBLE_LPAREN
%token DOUBLE_RPAREN
%token REDIRECT_OUTPUT
%token REDIRECT_IO_DOC
%token REDIRECT_IO_STR
%token NAME
%token NUM
%token SINGLE
%token WORD
%token PATTERN

%union {
  struct Word word;
  struct Atom *atom;
  struct Expression *expr;
  struct Pattern *pattern;
}

%token <word> WORD
%token <atom> ATOM
%token <pattern> PATTERN

%type <atom> command pipeline conditional loop case_atom literal substitution_atom pattern_atom
%type <expr> expression
%type <Word> WORD
%type <Word> NUM

%%


program: /* empty */
       | program line
       ;

line: command_list '\n'
    | '\n'
    ;

command_list: command
           | command_list ';' command
	   ;

command: simple_command
       | command '|' command
       | command '&' command
       ;

simple_command: WORD
              | simple_command WORD
              | simple_command redirection
              | simple_command pattern_atom
              | simple_command substitution_atom
	      ;

redirection: '<' WORD
           | '>' WORD
           | REDIRECT_OUTPUT WORD
	   | NUM '<' WORD
	   | NUM '>' WORD
	   | NUM REDIRECT_OUTPUT WORD
	   ;

io_here: command here_word '\n' word_list WORD
       | command here_word
       ;

here_word: REDIRECT_IO_STR WORD
         | REDIRECT_IO_DOC WORD
         ;

function: NAME OPEN_CLOSE '{' line '}'
	;

pattern_atom: PATTERN
	    ;

substitution_atom: ATSIGN_LCURLY command_list '}'
                | DOLLAR_LPAREN command_list ')'
		| DOLLAR_LCURLY param substitution_pattern '}'
		| '$' DOUBLE_LPAREN expression DOUBLE_RPAREN
                | '$' NAME
                | substitution_atom pattern_atom
		;

substitution_pattern: ":?" glob_pattern
		    | ":+" glob_pattern
		    | ":-" glob_pattern
		    | ":=" glob_pattern
                    | "##" glob_pattern
		    | "%%" glob_pattern
		    | '?'  glob_pattern
		    | '+'  glob_pattern
		    | '-'  glob_pattern
		    | '='  glob_pattern
		    | '#'  glob_pattern
		    | '%'  glob_pattern
		    ;

glob_pattern: regopt '*'
	    | regopt '?'
	    | regopt '+'
	    ;

regopt: '[' WORD ']'
      | '[' PATTERN '-' PATTERN ']'
      | WORD
      ;

pipeline: simple_command
        | pipeline '|' simple_command
	;

conditional: IF command_list THEN command_list elif ELSE command_list FI
           | IF command_list THEN command_list elif FI
	   ;

elif: 
    | ELIF command_list THEN command_list
    | ELIF command_list THEN command_list elif
    ;

loop: WHILE command_list DO command_list DONE
    | UNTIL command_list DO command_list DONE
    | FOR WORD IN word_list DO command_list DONE
    ;

case_atom: CASE WORD IN case_list ESAC	 { $$ = make_case($4, $2; 	            }
	 ;

case_list: pattern_list command_list case_list { $$ = make_case_list($3, $1, $2);   }
         | /* empty */
	 ;

pattern_list: pattern_list '|' pattern  { $$ = make_pattern_list($1, $3);           }

            | pattern			{ $$ = make_pattern_list(NULL, $1);         }
	    ;

pattern: PATTERN			{ $$ = make_pattern(NULL, $1);		    }
       | PATTERN '|' pattern		{ $$ = make_pattern($3, $1);		    }
       ;

word_list: word_list WORD		{ $$ = make_word_list($1, $2);		    }
         | WORD				{ $$ = make_word_list(NULL, $1);	    }
	 ;


expression: NUM '+' expression		{ $$ = make_expression($3, $1, EXPR_ADD);   }
	  | NUM '-' expression		{ $$ = make_expression($3, $1, EXPR_SUB);   }
	  | NUM '*' expression		{ $$ = make_expression($3, $1, EXPR_MUL);   }
	  | NUM '/' expression		{ $$ = make_expression($3, $1, EXPR_DIV);   }
	  | NUM '%' expression		{ $$ = make_expression($3, $1, EXPR_MOD);   }
	  | NUM ">>" expression		{ $$ = make_expression($3, $1, EXPR_SHR);   }
	  | NUM "<<" expression		{ $$ = make_expression($3, $1, EXPR_SHL);   }
	  | NUM "**" expression		{ $$ = make_expression($3, $1, EXPR_POW);   }
	  | NUM				{ $$ = make_expression(NULL, $1, EXPR_NUM); }
	  ;













