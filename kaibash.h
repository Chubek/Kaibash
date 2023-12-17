#ifndef KAIBASH_H_
#define KAIBASH_H_

#define UNREACHABLE		(0)

#define ESC_DQUOTE              1
#define ESC_SQUOTE              2
#define ESC_BACKTICK            4
#define ESC_SOLIDUS             8
#define ESC_TILDE               16

#define Word			char*
#define Script			char*
#define ScripPath		char*
#define Repl			char*

typedef struct Atom 		Atom;
typedef struct Pattern		Pattern;
typedef struct Sequence		Sequence;
typedef struct Pipeline		Pipeline;
typedef struct Shell		Shell;
typedef struct Input		Input;
typedef struct virtBuf		VirtBuf;

struct Atom
{
        struct Atom*    next;

        enum Kind
        {
                ASSIGNMENT_WORD,
                WORD,
                NAME,
                SEPARATOR,
                IO_NUMBER,
                GROUPER,
        }               kind;
enum WordKind
        {
                IO_HERE_WORD,
                CMD_NAME_WORD,
                CMD_WORD_WORD,
                CMD_SUFFIX_WORD,
                CMD_PREFIX_WORD,
                WORD_LIST_WORD,
                PATTERN_WORD,
                CASE_WORD,
                FILENAME_WORD,
                ESC_WORD,
        }               word_kind;
        enum NameKind
        {
                FOR_NAME,
                FUNC_NAME,
                KEYWORD_NAME,
        }               name_kind;
        enum SeparatorKind
        {
                AMPERSAND,
		SEMICOLON,
                DOUBLE_SEMICOLON,
                LESS_AND,
                LESS,
                GREAT,
                GREAT_AND,
                CLOBBER,
                LESS_GREAT,
                DOUBLE_GREAT,
                DOUBLE_LESS,
                DOUBLE_LESS_DASH,
                SHELL_PIPE,
		SHELL_PIPE_AND,
                AND_IF,
                OR_IF,
                PCNT,
                DOUBLE_PCNT,
                POUND,
                DOUBLE_POUND,
		COLON,
		QMARK,
		PLUS,
		DASH,
		EQSIGN,
		QMARK_COLON,
		PLUS_COLON,
		DASH_COLON,
		EQSIGN_COLON,
        }               separator_kind;
        enum GrouperKind
        {
                PAREN_OPEN,
                PAREN_CLOSE,
                DOUBLE_PAREN_OPEN,
                DOUBLE_PAREN_CLOSE,
                CURLY_OPEN,
                CURLY_CLOSE,
                DOUBLE_CURLY_OPEN,
                DOUBLE_CURLY_CLOSE,
		SQUARE_OPEN,
		SQUARE_CLOSE,
		DOUBLE_SQUARE_OPEN,
		DOuBLE_SQUARE_CLOSE,
		ANGLE_OPEN,
		ANGLE_CLOSE,
		DOUBLE_ANGLE_OPEN,
		DOUBLE_ANGLE_CLOSE,
		
        }               grouper_kind;
        enum KeywordKind
        {
                IF,
                ELIF,
                ELSE,
                FOR,
                CASE,
                ESAC,
                DO,
                DONE,
                WHILE,
                UNTIL,
        }               keyword_kind;

        int             io_num_val;
        Word            nwval_1;
        Word            nwval_2;
	Pattern*	pval;
        size_t          lnwval_1;
        size_t          lnwval_2;
        bool            paramd;
        int             escape_kind;
};

#define	PATT_LITERAL		1
#define	PATT_WIRLDCARD		2
#define	PATT_GROUP		4
#define PATT_REGULAR		8

struct Pattern
{
	enum PatternSigil
	{
		EXCLAIM,
		QMARK,
		STAR,
		ATSIGN,
		PLUS,
	}		sigil;

	int		kind;
	char*		value;
	size_t          length;
}

struct Sequence
{
	struct Sequence*	next;
	Word			program;
	Word			arguments[ARG_MAX];

	enum SequenceKind
	{
		REDIR_INPUT,
		REDIR_OUTPUT,
		APPEND_OUTPUT,
		DUP_INPUT,
		DUP_OUTPUT,
		CLOBBER_OUTPUT,
		IO_HERE_DOC,
		IO_HERE_STR,
		OPEN_RW,
	}			sequence_kind;
	
	bool			parallelized;
	pid_t			process_id;
	pid_t			group_id;
	pid_t 			session_id;
	int			last_exit_stat;
	int			infile;
	int			outfile;
	int			errfile;
	int			dup_in;
	int			dup_out;
	int			dup_err;
	Word			here_word;
	
}

#define PIPE_NUM		2

struct Pipeline
{
	struct Pipeline*	next;
	struct Sequence*	sequence;
	bool			banged;
	bool			pipe_err;
	int			pipe_in_fd;
	int			pipe_out_fd;
	int			pipe_io[PIPE_NUM];
	int			pipe_err[PIPE_NUM];

}

typedef struct termios Termios;

struct Shell
{
	struct Pipeline*	pipelines;
	Termios			termio;
	int			termfd;
	bool			interactive;
	bool			login;
	pid_t			process_id;
	pid_t			group_id;
	pid_t			session_id;
}

struct Input
{
	Word			word;
	Script			script;
	ScriptPath		script_path;
	Repl			repl;

	enum InputKind
	{
		WORD_IN,
		SCRIPT_IN,
		PATH_IN,
		REPL_IN,
	}			input_kind;
}

#endif
