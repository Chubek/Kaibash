%{
#include "tables.h"
#include "kaibash.h"


%}

%token PREFIX
%token COMMAND_WORD
%token QUOTED_STRING
%token IO_HERE_DOC
%token IO_HERE_STR
%token REDIR
%token HERE_WORD
%token HERE_CONTENT
%token IN_REDIR_NUM
%token IN_REDIR_WORD
%token IN_REDIR_OP
%token OUT_REDIR_OP
%token OUT_REDIR_NUM
%token OUT_REDIR_WORD
%token NEWLINE
%token NAME
%token ASSIGNMENT_WORD
%token CASE
%token WORD
%token DSEMI
%token ESAC
%token PATTERN
%token IF
%token IF_PATTERN
%token THEN
%token ELSE
%token FI
%token ELIF
%token ELIF_PATTERN
%token FOR
%token FOR_NAME
%token IN
%token FOR_THIRD_WORD
%token DO
%token DONE
%token WHILE
%token UNLESS
%token DO
%token DO
%token LOOP_WORD
%token COLON_PLUS
%token COLON_DASH
%token COLON_EQUAL
%token COLON_QMARK
%token DOUBLE_LPAREN
%token DOUBLE_LCURLY
%token DOUBLE_RPAREN
%token DOUBLE_RCURLY

%%


















