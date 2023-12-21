%{
#include "kaibash.h"
#include "tables.h"

#undef YY_INPUT
#define YY_INPUT(b, r, ms)		(r = kaibash_yyinput(b, ms))

extern void   output_location_info(void);

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

<COMMAND_BODY>"<<"       {
			    BEGIN(HERE_DOC);
			    return IO_HERE_DOC;
			 }

<COMMAND_BODY>"<<<"      {
			    BEGIN(HERE_STR);
			    return IO_HERE_STR;
			 }

<COMMAND_BODY>">>"	 {
			    BEGIN(REDIR_WORD);
			    return REDIR;
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

<COMMAND_BODY>[0-9]+ / [<&] { 
			       BEGIN(IN_REDIR);
			       yylval.io_num = strtod(&yytext[0], NULL, 10);
			       return IN_REDIR_NUM;
			    }
<COMMAND_BODY>{Word} / [<&] { 
			       BEGIN(IN_REDIR);
			       yylval.io_word = STRNDUP(&yytext[0], yyleng);
			       return IN_REDIR_WORD;
			    }

<IN_REDIR>[&<]		    {
				BEGIN(COMMAND_BODY);
				yylval.in_redir_op = STRNDUP(&yytext[0], yyleng);
				return IN_REDIR_OP;
			    }

<COMMAND_BODY>[>&] / [0-9]+ {
				BEGIN(OUT_REDIR);
				yylval.out_redir_op = STRNDUP(&yytext[0], yyleng);
				return OUT_REDIR_OP;
			    }


<OUT_REDIR>[0-9]+	    {
				BEGIN(COMMAND_BODY);
				yylval.io_num = strtod(&yytext[0], NULL, 10);
				return OUT_REDIR_NUM;
			    }

<OUT_REDIR>{Word}	    {
				BEGIN(COMMAND_BODY);
				yylval.io_word = STRNDUP(&yytext[0], yyleng);
				return OUT_REDIR_WORD;
			    }

<COMMAND_BODY>[|;&]         return *yytext;

<COMMAND_BODY>[\n]	    {
				BEGIN(INITIAL);
				return NEWLINE;
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

<CASE_BODY>)		     return *yytext;

<CASE_BODY>";;"              return DSEMI;

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

<IF_INIT,ELIF_INIT>[[\];]   return *yytext;


<IF_INIT>{Word}          {
			    yylval.if_pattern = STRNDUP(&yytext[0], yyleng);
			    return IF_PATTERN;
                         }

<IF_BODY>"then"	         {
			    BEGIN(IF_BODY);
			    return THEN;
			 }

<IF_BODY,ELIF_BODY>"else"   return ELSE;

<IF_BOFY,ELIF_BODY>"fi"  {
			    BEGIN(INITIAL);
			    return FI;
                         }

<IF_BODY>"elif"	         {
			    BEGIN(ELIF_INIT);
			    return ELIF;
			 }

<ELIF_INIT>"then"	 {
			    BEGIN(ELIF_BODY);
			 }


<ELIF_INIT>{Word}	 {
			    yylval.elif_pattern = STRNDUP(&yytext[0], yyleng);
			    return ELIF_PATTERN;
			 }

"for"			 {
			    BEGIN(FOR_INIT);
			    return FOR;
			 }

<FOR_INIT>{Name}	 {
			    yylval.for_name = STRNDUP(&yytext[0], yyleng);
			    return FOR_NAME;
			 }

<FOR_INIT>"in"	         {
			    BEGIN(FOR_THIRD);
			    return IN;
			 }

<FOR_THIRD>[{}]	            return *yytext;

<FOR_THIRD>{Word}	 {
			    yylval.for_third_word = STRNDUP(&yytext[0], yyleng);
			    return FOR_THIRD_WORD;
			 }

<FOR_THIRD>"do"	         {
			    BEGIN(FOR_BODY);
			    return DO;
			 }



<FOR_BODY,DO_BODY,UNLESS_BODY>"done" {
					BEGIN(INITIAL);
					return DONE;
				     }

"while"			{
			   BEGIN(WHILE_INIT);
			   return WHILE;
		        }

"unless"		{
			   BEGIN(UNLESS_INIT);
			   return UNLESS;
			}

<WHILE_INIT>"do"	{
			   BEGIN(WHILE_BODY);
			   return DO;
			}

<UNLESS_INIT>"do"       {
			   BEGIN(UNLESS_BODY);
			   return DO;
			}

<WHILE_INIT,UNLESS_INIT>{Word}	{
			   	  yylval.loop_word = STRNDUP(&yytext[0], yyleng);
			  	  return LOOP_WORD;
		          	}

[`"'$~:?+-=%/(){}]	return *yytext;

":+"			return COLON_PLUS;
":-"			return COLON_DASH;
":="			return COLON_EQUAL;
":?"			return COLON_QMARK;

"(("			return DOUBLE_LPAREN;
"{{"			return DOUBLE_LCURLY;
"))"			return DOUBLE_RPAREN;
"}}"			return DOUBLE_RCURLY;

%%

extern char	kaibash_input[];
extern char*	kaibash_inputptr;
extern int*	kaibash_inputdelim;

void
yyerror(const char* msg)
{
	fputs("Syntax error occured: ", stderr);
	fputs(msg, stderr);
	fputs("\n", stderr);

	output_location_info();
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
