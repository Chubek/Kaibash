%{
#include "y.tab.h"
#include "tables.h"

#undef YY_INPUT
#define YY_INPUT(b, r, ms)		(r = kaibash_yyinput(b, ms))

int yyerror(const char*);

long nword = 0, nline = 0;	

%}

Pattern [^[:space:]]+
Word [a-zA-Z0-9_./+-]+
Num [0-9]+
Name [a-zA-Z_][a-zA-Z_0-9]*

%%

\n          { nline++; return '\n'; }
[ \t]       { nword++; }
";"         { return SEMICOLON; }
"|"         { return PIPE; }
"&"         { return AMPERSAND; }
"if"        { return IF; }
"then"      { return THEN; }
"else"      { return ELSE; }
"fi"        { return FI; }
"while"     { return WHILE; }
"until"     { return UNTIL; }
"do"        { return DO; }
"done"      { return DONE; }
"for"       { return FOR; }
"in"        { return IN; }
"case"      { return CASE; }
"esac"      { return ESAC; }
"<"         { return REDIRECT_INPUT; }
">"         { return REDIRECT_OUTPUT; }
">>"        { return REDIRECT_APPEND; }
"{"         { return '{'; }
"}"         { return '}'; }
"@"         { return '@'; }
"("         { return '('; }
")"         { return ')'; }
"$"         { return '$'; }
{Num}       { yylval.word = STRNDUP(yytext, yyleng); return NUM; }
{Word}      { yylval.word = STRNDUP(yytext, yyleng); return WORD; }
{Name}	    { yylval.word = STRNDUP(yytext, yyleng); return NAME; }
{Pattern}   { yylval.pattern = STRUNDUP(yytext, yyleng); return PATTERN; }

.           { return *yytext; }

%%

extern char	kaibash_input[];
extern char*	kaibash_inputptr;
extern int*	kaibash_inputdelim;

void
yyerror(const char* msg)
{
	fputs("Syntax error occured: ", stderr);
	fputs(msg, stderr);	
	fprintf(stderr, "; At line: %ld, word: %ld\n", nline, nword);
}

int
kaibash_yyinput(char* buf, int max_size)
{
	int n = min(max_size, kaibash_inputlim - kaibash_inputptr);

	if (n > 0) {
		memmove(&buf[0], kaibash_inputptr, n);
		kaibash_inputptr += n;
	}

	return n;
}
