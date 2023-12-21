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

substitution_atom: '@' '{' command_list '}'
                | '$' '(' command_list ')'
		| '$' '{' param substitution_pattern '}'
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

conditional: IF command_list THEN command_list ELSE command_list FI
           | IF command_list THEN command_list FI
           | IF command_list THEN command_list ELSE conditional FI
	   ;

loop: WHILE command_list DO command_list DONE
    | UNTIL command_list DO command_list DONE
    | FOR WORD IN word_list DO command_list DONE
    ;

case_atom: CASE WORD IN case_list ESAC
	 ;

case_list: pattern_list command_list case_list
         | /* empty */
	 ;

pattern_list: pattern_list '|' pattern
            | pattern
	    ;

pattern: PATTERN
       | PATTERN '|' pattern
       ;

word_list: word_list WORD
         | WORD
	 ;
















