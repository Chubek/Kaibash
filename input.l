%{
#include "kaibash.h"
#include "tables.h"

enum InputType
{
	SCRIPT, 
	INLINE,
	REPL,
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

Name [a-zA-Z_][a-zA-Z0-9]*
Word [a-zA-Z0-9~!@`#$%^*()-=[\]':"/?.,]+
Pfix ^(/([a-zA-Z0-9_-]+/?)*)*

%x FOR_INIT
%x FOR_THIRD
%x FOR_BODY
%x CASE_INIT
%x CASE_BODY
%x IF_INIT
%x IF_BODY
%x ELIF_INIT
%x ELIF_BODY
%x WHILE_INIT
%x WHILE_BODY
%x UNLESS_INIT
%x UNLESS_BODY
%x FUNCTION_BODY
%x COMMAND_BODY
%x NAME_INIT
%x HERE_IO

%%


<FOR_BODY,CASE_BODY,IF_BODY,ELIF_BODY,WHILE_BODY,UNLESS_BODY,INITIAL,FUNCTION_BODY>{Pfix} {
		BEGIN(COMMAND_BODY);
		yylval.prefix = STRNDUP(&yytext[0], yyleng);
		return PREFIX;
}

<COMMAND_BODY>{Word}     {
                            yylval.command_word = STRNDUP(&yytext[0], yyleng);
			    return COMMAND_WORD;
			 }

<COMMAND_BODY>[\n|;&]    {
			    BEGIN(INITIAL);
			    return *yytext;
			 }

<COMMAND_BODY>"<<"       {
			    BEGIN(HERE_DOC);
			    return IO_HERE_DOC;
			 }

<COMMAND_BODY>"<<<"      {
			    BEGIN(HERE_STR);
			    return IO_HERE_STR;
			 }

<HERE_IO>{Word} / "\n"   {
			    BEGIN(HERE_CONTENT);
			    yylval.here_word = STRNDUP(&yytext[0], yyleng);
			    return HERE_WORD;
	                 }

<HERE_CONTENT>\n[^\n]/\n {
			    yylval.here_content = STRNDUP(&yytext[1], yyleng);
			    return HERE_CONTENT;
			 }

{Name} / "="		 {
                            BEGIN(NAME_INIT);
			    yylval.name = STRNDUP(&yytext[0], yyleng);
			    return NAME;
			 }

<NAME_INIT>"="{Word}     { 
			     BEGIN(INITIAL);
			     yylval.assignment_word = STRNDUP(&yytext[1], yyleng -1);
			     return ASSIGNMENT_WORD;
			 }

"case"			 {
                             BEGIN(CASE_INIT);
			     return CASE;
			 }

<CASE_INIT>{Word}	 {
			     BEGIN(CASE_BODY);
			     yylval.case_word = STRNDUP(&yytext[0], yyleng);
			     return WORD;
			 }

<CASE_BODY>)		 return *yytext;

<CASE_BODY>";;"          return DSEMI;

<CASE_BODY>"esac"	 {   
                            BEGIN(INITIAL);
			    return ESAC;
			 }


<CASE_BODY>{Word} / ")"	 {
			    yylval.body_word = STRNDUP(&yytext[0], yyleng);
			    return PATTERN;
			 }

"if"			 {
			    BEGIN(IF_INIT);
			    return IF;
			 }

<IF_INIT>[[\]]           return *yytext;

<IF_INIT>{Word}          {
			    yylval.if_pattern = STRNDUP(&yytext[0], yyleng);
			    return IF_PATTERN;
                         }

<IF_BODY>"then"	         {
			    BEGIN(IF_BODY);
			    return THEN;
			 }

<IF_BODY>"else"	         return ELSE;

<IF_BOFY>"fi"            {
			    BEGIN(INITIAL);
			    return FI;
                         }
