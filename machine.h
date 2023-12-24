#ifndef STACK_SIZE
#define STACK_SIZE			(1 << 16)
#endif

#ifndef WORD_SIZE
#define WORD_SIZE			(1 << 12)
#endif

#ifndef NAME_SIZE
#define NAME_SIZE			(1 << 8)
#endif

typedef int FDesc;
typedef int PosParam;

enum Opcode
{
	OPCODE_INIT,
	OPCODE_WORD,
	OPCODE_EXECUTE,
	OPCODE_REDIRECT,
	OPCODE_DUPLICATE,
	OPCODE_PIPE_IO,
	opCODE_PIPE_IO_AND_ERR,
	OPCODE_RUN_FOREGROUND,
	OPCODE_SUBSTITUTE_PATTERN,
	OPCODE_LOAD_VAR,
	OPCODE_STORE_VAR,
	OPCODE_LOAD_FUNCTION,
	OPCODE_STORE_FUNCTION,
	OPCODE_LOAD_PARAMETER,
	OPCODE_STORE_PARAMETER,
	OPCODE_LOAD_FILE_DESCRIPTOR,
	OPCODE_STORE_FILE_DESCRIPTOR,
	OPCODE_PUSH_STR,
	OPCODE_PUSH_FILE_DESCRIPTR,
	OPCODE_PUSH_PREFIX,
	OPCODE_PUSH_ARGUMENTS,
	OPCODE_TEST_PATTERN,
	OPCODE_TEST_EXPR,
	OPCODE_TERMINATE_HERE,
	OPCODE_FILE_PATH,
	OPCODE_SUBSHELL,
	OPCODE_END,
};

enum PatternNodeKind {
	PATTNODE_LITERAL,
	PATTNODE_VARIABLE,
	PATTNODE_WILDCARD,
	PATTNODE_SEQUENCE,
	PATTNODE_ALTERNATION,
};


struct Pattern {
    enum PatternNodeKind kind;
    union {
        char* literal;            
        char* variable_name;      
        struct Pattern* children; 
    };
};


enum ValueKind
{
	VALUE_WORD,
	VALUE_OPCODE,
	VALUE_ARG,
	VALUE_NAME,
	VALUE_FDESC,
	VALUE_PATTERN,
	VALUE_PARAMETER,
	VALUE_SPECIAL_PARAMETER,
	VALUE_POSITIONAL_PARAMETER,
};

enum SpecialParamKind
{
	IFS_AWARE_ARGV,
	IFS_NONAWARE_ARGV,
	ARGC_NUM,
	LAST_EXIT_STAT,
	CURR_OPT_FLAG,
	LAST_INVOKED_PID,
	LAST_BG_CMD,
	SCRIPT_NAME,
};

struct Word
{
	char container[WORD_SIZE];
	size_t length;
};

struct Name
{
	char container[NAME_SIZE];
	size_t length;
};


struct Arguments
{
	char arguments[WORD_SIZE][ARG_MAX];
	size_t num_arg;
}

union Value
{
	struct Word* word;
	struct Name* name;
	struct Name* parameter;
	struct Pattern* pattern;
	struct Arguments* args;
	struct SpecialParamKind special_param;
	enum Opcode opcode;
	PosParam pos_param;
	Fdesc fdesc;
};

struct Value
{
	enum ValueKind kind;
	union Value value;
};


struct Stack
{
	struct Value* stack[STACK_SIZE];
	int top;
};





