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
%token REDIRECT
%token WORD
%token PATTERN

%union {
  struct Word word;
  struct Atom *atom;
  struct Pattern *pattern;
}

%token <word> WORD
%token <atom> ATOM
%token <pattern> PATTERN

%type <atom> command pipeline conditional loop case_atom literal substitution_atom pattern_atom

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
           | '>>' WORD
	   ;

pattern_atom: PATTERN
	    ;

substitution_atom: '@' '{' command_list '}'
                | '$' '(' command_list ')'
                | '$' WORD
                | substitution_atom pattern_atom
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
















