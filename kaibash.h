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

struct CommandRedirection
{
    enum RedirectionType
    {
        REDIRECT_INPUT,
        REDIRECT_OUTPUT,
        REDIRECT_APPEND,
        REDIRECT_INPUT_FD,
        REDIRECT_OUTPUT_FD,
        REDIRECT_APPEND_FD,
        HERE_STRING,
        HERE_DOC,
    } redirection_type;

    Word source;
    Word destination;

    Word here_doc_marker;
    struct Atom* here_doc_content;
};

struct Atom
{
	struct Atom*		next_atom;

	enum AtomKind
	{
		COMMAND,
		PIPELINE,
		CONDITIONAL,
		LITERAL,
		LOOP,
		CASE,
		PATTERN,
		SUBSTITUTION,
	}			atom_kind;

	union AtomNode 
	{
		struct CommandAtom
		{
			char* command;
			char* arguments[ARG_MAX];
                        struct CommandRedirection* redirections;
		
		}		command_atom;

		struct PipelineAtom
		{
			struct Atom* left;
			struct Atom* right;
		
		}		pipeline_atom;

		struct ConditionalAtom
		{
			struct Atom* condition;
			struct Atom* true_branch;
			struct Atom* false_branch;
		
		}		conditional_atom;

		struct CaseAtom
		{
			Word	word;
			struct Atom* list;
			struct Atom* cases;
		
		}		case_atom;

		struct LoopAtom
		{
			struct Atom* condition;
			struct Atom* body;
			bool   unless;
		
		}		loop_atom;

		struct LiteralAtom
		{
			enum LiteralKind
			{
				STRING,
				INTEGER,
			}	literal_kind;

			enum LiteralNode
			{
				char* string;
				intmax_t integer;
			}	literal_node;

		}		literal_atom;

		struct SubstitutionAtom
		{
			enum SubstitionKind
			{
				VARIABLE_SUBSTITUTION,
				COMMAND_SUBSTITUTION,
				ARITHMETIC_SUBSTITUTION,
				PARAMETER_SUBSTITUIN,
				BRACE_EXPRESSION,
				TILDE_EXPRESSION,
				LENGTH_SUBSTITUIN,
			}	substitution_kind;

			struct Atom* substitution_node;
		}		substitution_atom;


		Pattern*	pattern_atom;
	}
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

#define MAX_SEQ_KIND		16

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
		SEQ_KIND_TERM,
	}			sequence_kind[MAX_SEQ_KIND];
	
	bool			parallelized;
	pid_t			process_id;
	pid_t			group_id;
	pid_t 			session_id;
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
	int			pipe_err_fd;
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
	int			last_exit_stat;
}


#endif
