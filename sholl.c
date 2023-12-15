#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/*======== Memory allocation table =======*/

// This struct is a value on its own, a singly-linked list, it holds the allocation table
static struct Memory 
{
	struct Memory* 	next;
	void*		memv;		// allocated value
	size_t		size;		// its size
	int		nmem;		// number of members
	bool		free;		// whether it's freed (unused)
}
*MEMORY;


// this function is used to allocate memory and get the pointer

static inline void*
allocate_memory(void* value, int nmem, size_t size)
{
	struct Memory* node = calloc(1, sizeof(*MEMORY));
	node->memv          = calloc(nmem, size);
	node->nmem	    = nmem;
	node->free          = false;
	node->next          = MEMORY;
	
	memmove(&node->memv, &value, nmem * size);

	MEMORY  	    = node;
}

// This function is used to dump the static allocation table (the allocation table is static, not the allocated values!)
static inline void
dump_memory(void)
{
	for (struct Memory* mem = MEMORY;
			mem != NULL;
			mem  = mem->next)
	{
		if (!mem->free)
		{
			free(mem->memv);
			mem->free = true;}
	}
}

/*======== Symbols table ========*/


// This is another list structure, it holds the symbols and their values. All symbols are strings
static struct Symtab
{	
	struct Symtab* 	next;
	char*	      	symr;		// symbol representation
	char*		symv;		// symbol value
	size_t          rlen;		// representation length
	size_t 		vlen;		// value length


}
*SYMTAB;

// allocate a symbol in the table
static inline void
allocate_symbol(char* symr, char* symv)
{
	size_t lr, lv;
	lr = strlen(symr);
	lv = strlen(symv);
	
	struct Symtab*      = calloc(1, sizeof(*SYMTAB));
	node->symr	    = allocate_memory(symr, lr, 1);
	node->symv          = allocate_memory(symv, lv, 1);
	node->rlen	    = lr;
	node->vlen          = lv;
	node->next          = SYMTAB;

	SYMTAB		    = node;	
}

// get the symbol back
static inline char*
get_symbol(char* symr)
{
	for (struct Symtab* stab = SYMTAB;
			stab != NULL;
			stab  = stab->next)
	{
		if (!strncmp(stab->symr, symr, stab->rlen))
			return stab->symv;
	}
	return NULL;
}

/*==== end of boilerplate ====*/

#define STRDUP(s)	allocate_memory(s, 1, strlen(s))

#define ESC_DQUOTE		1
#define ESC_SQUOTE		2
#define ESC_BACKTICK		4
#define ESC_SOLIDUS		8
#define ESC_TILDE		16

typedef char* Word;

struct Atom
{
	struct Atom*	next;

	enum Kind
	{
		ASSIGNMENT_WORD,
		WORD,
		NAME,
		SEPARATOR,
		IO_NUMBER,
		GROUPER,
	}		kind;
	enum WordKind
	{
		IO_HERE_WORD,
		CMD_NAME_WORD,
		CMD_WORD_WORD,
		CMD_SUFFIX_WORD,
		CMD_PREFIX_WORD,
		WORDLIST_WORD,
		PATTERN_WORD,
		CASE_WORD,
		FILENAME_WORD,
		ESCAPED_WORD,
	}		word_kind;
	enum NameKind
	{
		FOR_NAME,
		FUNC_NAME,
		KEYWORD_NAME,
	}		name_kind;
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
		AND_IF,
		OR_IF,
		PCNT,
		DOUBLE_PCNT,
		POUND,
		DOUBLE_POUND,
	}		separator_kind;
	enum GROUPER_KIND
	{
		PAREN_OPEN,
		PAREN_CLOSE,
		DOBULE_PAREN_OPEN,
		DOUBLE_PAREN_CLOSE,
		CURLY_OPEN,
		CURLY_CLOSE,
		DOUBLE_CURLY_OPEN,
		DOUBLE_CURLY_CLOSED,
	}		grouper_kind;
	enum KEYWORD_KIND
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
	}		keyword_kind;

	int 		io_num_val;
	char*		nwval_1;
	char*		nwval_2;
	bool 		parameterized;
	int		escape_kind;
}

static inline struct Atom*
allocate_atom(void)
{
	struct Atom atom = {0};
	return allocate_memory(&atom, 1, sizeof(struct Atom));
}

static struct Atom*
atomize_fmt(char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	struct Atom* atom = allocate_atom();

	if (strstr(fmt, "io-word-here"))
	{
		char* arg  = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = IO_HERE_WORD;
		atom->nwval_1 = STRDUP(arg);

	}
	else if (strstr(fmt, "cmd-name-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = CMD_NAME_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "cmd-word-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = CMD_WORD_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "cmd-suffix-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = CMD_SUFFIX_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "cmd-prefix-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = CMD_PREFIX_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "word-list-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = WORD_LIST_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "pattern-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = PATTERN_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "case-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = CASE_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "filename-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = FILENAME_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "escaped-word"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = WORD;
		atom->word_kind = ESCAPED_WORD;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "for-name"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = NAME;
		atom->name_kind = FOR_NAME;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "func-name"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = NAME;
		atom->name_kind = FUNC_NAME;
		atom->nwval_1 = STRDUP(arg);
	}
	else if (strstr(fmt, "keyword-name"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = NAME;
		atom->name_kind = KEYWORD_NAME;
		atom->keyword_kind = get_keyword_kind(arg);
	}
	else if (strstr(fmt, "separator"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = SEPARATOR;
		atom->separator_kind = get_seprator_kind(arg);
	}
	else if (strstr(fmt, "grouper"))
	{
		char* arg = va_arg(ap, char*);
		atom->kind = GROUPER;
		atom->grouper_kind = get_grouper_kind(arg);
	}
	else if (strstr(fmt, "assign-word"))
	{
		char* arg1 = va_arg(ap, char*);
		char* arg2 = va_arg(ap, char*);
		atom->kind = WORD_ASSIGN;
		atom->nwval1 = arg1;
		atom->nwval2 = arg2;
	}
	
	if (strstr(fmt, "param"))
		atom->parametrized = true;
	
	if (strstr(fmt, "esc-dquote"))
		atom->escape_kind = ESCAPE_DQUOTE;
	else if (strstr(fmt, "esc-squote"))
		atom->escape_kind = ESCAPE_SQUOTE;
	else if (strstr(fmt, "esc-backtick"))
		atom->escape_kind = ESCAPE_BACKTICK;
	else if (strstr(fmt, "esc-tilde"))
		atom->escape_kind = ESCAPE_TILDE;
	else if (strstr(fmt, "esc-solidus"))
		atom->escape_kind = ESCAPE_SOLIDUS;
	
	if (strstr(fmt, "next-swap"))
	{
		struct Atom** arg = va_arg(ap, struct Atom**);
		atom->next = *arg;
		*arg = atom;
	}
	return atom;
}

static inline int
simple_FEW_loop(char* prog, char** argv)
{
	pid_t child_pid;
	int   exit_stat;

	if ((child_pid = fork()) < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (!child_pid)
	{
		if (execvp(prog, argv) < 0)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}

	return (waitpid(child_pid, &exit_state, 0) == child_pid)
		? exit_state
		: EXIT_FAILURE;
}
enum IoMode { INTO, UNTO, CLOBBER, APPEND, DUP, RW };
	    /* >     <      >|      >>      >&  <> */

static inline int
io_FEW_loop(char* prog, 
		char** argv, 
		int fd,
		int fd_copy,
		bool no_clobber,
		char* filename, 
		enum IoMode mode)

{
	int fcntl_flags = -1;
	int dest_fdesc  = -1;
	mode_t fcntl_modes = 0;

	switch (mode)
	{
		case INTO:
		case DUP:
			fcntl_flags = no_clobber ? 0 : O_WRONLY | O_CREAT | O_EXCL;
			fcntl_modes = no_clobber ? 0 : S_IWUSR | S_IRUSR;
			break;
		case UNTO:
			fcntl_flags = O_RDONLY | O_EXCL;
			break;
		case CLOBBER:
			fcntl_flags = O_WRONLY | O_CREAT | O_EXCL;
			fcntl_modes = S_IWUSR | S_IRUSR;
			break;
		case APPEND:
			fcntl_flags = O_APPEND | O_EXCL;
			break;
		case RW:
			fcntl_flags = O_RDWR | O_EXCL;
			break;
		default:
			break;
	}

	dest_fdesc = fcntl_modes 
			? open(filename, fcntl_flags, fcntl_modes)
			: open(filename, fcntl_flags);

	if (dest_fdesc < 0)
		perror("open");
	
	if (fd > 0)
		dup2(dest_fdesc, fd) < 0 ? perror("dup2") : ();
	if (fd_copy > 0)
		dup2(fd, fd_copy) < 0 ? perror("dup2") : ();

	int child_pid = 0;
	int wait_result = 0;

	if ((child_pid = fork()) < 0)
		perror("fork");

	else if (!child_pid)
	{
		if (execvp(prog, argv) < 0)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}

	return (waitpid(child_pid, &exit_state, 0) == child_pid)
		? exit_state
		: EXIT_FAILURE;
}

