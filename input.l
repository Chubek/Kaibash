%{
#include "kaibash.h"
#include "tables.h"

enum InputType
{
	SCRIPT, 
	INLINE,
} 			input_type;

#undef YY_INPUT
#define YY_INPUT(b, r, ms)		(r = kaibash_yyinput(b, ms))

static struct Input* current_input;
static bool is_repl = false;

FILE*	      yyin;
FILE*	      yyout;

int	      yylex(void);
int	      yyerror(const char*);

%}
