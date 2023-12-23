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

pipeline: simple_command		{ $$ = make_atom(ATOM_CMD, $1);	      }
        | pipeline '|' simple_command	{ $$ = add_command_pipeline(&$1, $3); }
	;

redirection: '<' WORD				{ $$ = make_redirect(REDIR_INPUT, NULL, $3);  }
           | '>' WORD				{ $$ = make_redirect(REDIR_OUTPUT, NULL, $3); }
           | REDIRECT_OUTPUT WORD		{ $$ = make_redirect(APPEND_OUTPUT, NULL, $3);}
	   | NUM '<' WORD			{ $$ = make_redirect(REDIR_INPUT, $1, $3);   }
	   | NUM '>' WORD			{ $$ = make_redirect(REDIR_OUTPUT, $1, $3);  }
	   | NUM REDIRECT_OUTPUT WORD		{ $$ = make_redirect(APPEND_OUTPUT, $1, $3); }
	   ;

io_here: command here_word '\n' word_list WORD	{ $$ = make_here_doc($1, $2, $4, $5); 	}
       | command here_word			{ $$ = make_here_str($1, $2); 		}
       ;

here_word: REDIRECT_IO_STR WORD { $$ = make_atom(HERE_STR_WORD, $2); }
         | REDIRECT_IO_DOC WORD	{ $$ = make_atom(HERE_DOC_WORD, $2); }
         ;

function: NAME OPEN_CLOSE '{' line '}' { $$ = make_function($1, $4); }
	;

pattern_atom: PATTERN		{ $$ = make_atom(ATOM_PATT, $1); }
	    ;

substitution_atom: DOLLAR_LCURLY command_list '}'         { $$ = make_sub_atom(SUB_SEQ, $1, NULL); }
                | DOLLAR_LPAREN command_list ')'
		| DOLLAR_LCURLY param substitution_pattern '}' { $$ = make_sub_atom(SUB_SHELL, $2, NULL); }
		| '$' DOUBLE_LPAREN expression DOUBLE_RPAREN { $$ = make_sub_atom(SUB_PATT, 
								$2, $3); }
                | '$' NAME			 { $$ = make_sub_atom(SUB_PARAM, $2, NULL); }
                | substitution_atom pattern_atom { $$ = make_subs_atom(SUB_CONCAT, $1, $2); }
		;

substitution_pattern: ":?" glob_pattern { $$ = make_subpatt(SUBPATT_QMARK_COLON, $2);  }
		    | ":+" glob_pattern { $$ = make_subpatt(SUBPATT_PLUS_COLON, $2);   }
		    | ":-" glob_pattern { $$ = make_subpatt(SUBPATT_DASH_COLON, $2);   }
		    | ":=" glob_pattern { $$ = make_subpatt(SUBPATT_EQUAL_COLON, $2);  }
                    | "##" glob_pattern { $$ = make_subpatt(SUBPATT_SUFFIX_LARGE, $2); }
		    | "%%" glob_pattern { $$ = make_subpatt(SUBPATT_PREFIX_LARGE, $2); }
		    | '?'  glob_pattern { $$ = make_subpatt(SUBPATT_QUESTION_MARK, $2);}
		    | '+'  glob_pattern { $$ = make_subpatt(SUBPATT_PLUS_SIGN, $2);    }
		    | '-'  glob_pattern { $$ = make_subpatt(SUBPATT_DASH_SIGN, $2);    }
		    | '='  glob_pattern	{ $$ = make_subpatt(SUBPATT_EQUAL_SIGN, $2);   }
		    | '#'  glob_pattern { $$ = make_subpatt(SUBPATT_SUFFIX_SMALL, $2); }
		    | '%'  glob_pattern { $$ = make_subpatt(SUBPATT_PREFIX_SMALL, $2); }
		    ;

glob_pattern: regopt '*'		{ $$ = make_glob(GLOB_KLEENE, $1);	    }
	    | regopt '?'		{ $$ = make_glob(GLOB_OPT, $1);		    }
	    | regopt '+'		{ $$ = make_glob(GLOB_CLOSURE, $1);	    }
	    | regopt			{ $$ = make_glob(GLOB_SIMPLE, $1);          }
	    ;

regopt: '[' WORD ']'			{ $$ = make_regopt(REGOPT_GROUP, $2, NULL); }
      | '[' PATTERN '-' PATTERN ']'     { $$ = make_regopt(REGOPT_RANGE, $2, $4);   }
      | WORD				{ $$ = make_regopt(REGOPT_WORD, $1, NULL);  }
      ;


conditional: IF command_list THEN command_list elif ELSE command_list FI {
							$$ = make_conditional($2, $4 $5, $6);
									 }
           | IF command_list THEN command_list elif FI { 
	   						 $$ = make_conditional($2, 
							 		$4, $5, NULL);
						       }
	   ;

elif: /* empty */
    | ELIF command_list THEN command_list	 { $$ = make_elif($2, $4);          }
    | ELIF command_list THEN command_list elif   { $$ = add_elif_list(&$5, $2, $4); }
    ;

loop: WHILE command_list DO command_list DONE    { $$ = make_while_loop($2, $4);    }
    | UNTIL command_list DO command_list DONE	 { $$ = make_until_loop($2, $4);    }
    | FOR WORD IN word_list DO command_list DONE { $$ = make_for_loop($2, $4, $6);  }
    ;

case_atom: CASE WORD IN case_list ESAC	 { $$ = make_case($4, $2); 	            }
	 ;

case_list: pattern_list ')' command_list case_list { $$ = add_case_list(&$0, $3, $1, $2);   }
         | /* empty */
	 ;

pattern_list: pattern_list '|' pattern  { $$ = add_pattern_list(&$1, $3);       }

            | pattern			{ $$ = make_atom(ATOM_PATT, $1);        }
	    ;

pattern: PATTERN			{ $$ = make_atom(ATOM_PATT_LIT, $1);	    }
       | PATTERN '|' pattern		{ $$ = add_pattern(&$3, $1);		    }
       ;

word_list: word_list WORD		{ $$ = add_word_list(&$1, $2);		    }
         | WORD				{ $$ = make_atom(ATOM_WORD_LIT, $1);	    }
	 ;


expression: NUM '+' expression		{ $$ = add_expression(&$3, $1, EXPR_ADD);   }
	  | NUM '-' expression		{ $$ = add_expression(&$3, $1, EXPR_SUB);   }
	  | NUM '*' expression		{ $$ = add_expression(&$3, $1, EXPR_MUL);   }
	  | NUM '/' expression		{ $$ = add_expression(&$3, $1, EXPR_DIV);   }
	  | NUM '%' expression		{ $$ = add_expression(&$3, $1, EXPR_MOD);   }
	  | NUM ">>" expression		{ $$ = add_expression(&$3, $1, EXPR_SHR);   }
	  | NUM "<<" expression		{ $$ = add_expression(&$3, $1, EXPR_SHL);   }
	  | NUM "**" expression		{ $$ = add_expression(&$3, $1, EXPR_POW);   }
	  | NUM				{ $$ = make_atom(ATOM_NUM_LIT, $1); }
	  ;













