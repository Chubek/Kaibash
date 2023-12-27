#ifndef MACHINE_H_
#define MACHINE_H_

#include <limits.h>

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
	OPCODE_EXECUTE_COMMAND,
	OPCODE_REDIRECT_IN,
	OPCODE_REDIRECT_OUT,
	OPCODE_REDIRECT_ERR,
	OPCODE_DUPLICATE_IN,
	OPCODE_DUPLICATE_OUT,
	OPCODE_DUPLICATE_ERR,
	OPCODE_REDIRECT_STR,
	OPCODE_PIPE_IO,
	opCODE_PIPE_IO_AND_ERR,
	OPCODE_RUN_IN_FOREGROUND,
	OPCODE_SUBSTITUTE_PATTERN,
	OPCODE_SUBSTITUTE_PARAM,
	OPCODE_LOAD_VAR,
	OPCODE_STORE_VAR,
	OPCODE_LOAD_SEQUENCE,
	OPCODE_STORE_SEQUENCE,
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
	OPCODE_PUSH_PATTERN,
	OPCODE_TEST_PATTERN,
	OPCODE_TEST_EXISTS,
	OPCODE_TEST_IS_REGULAR,
	OPCODE_TEST_IS_BLOCK,
	OPCODE_TEST_IS_CHAR,
	OPCODE_TEST_IS_GROUPID_SET,
	OPCODE_TEST_IS_USERID_SET,
	OPCODE_TEST_IS_STICKY_SET,
	OPCODE_TEST_IS_LARGER_ZERO,
	OPCODE_TEST_IS_TERMINAL,
	OPCODE_TEST_IS_STR_ZERO,
	OPCODE_TEST_IS_NAMED_PIPE,
	OPCODE_TEST_IS_SYMLINK,
	OPCODE_TEST_IS_DIRECTORY,
	OPCODE_TEST_IS_READABLE,
	OPCODE_TEST_IS_WRITABLE,
	OPCODE_TEST_IS_EXECUTABLE,
	OPCODE_TEST_IS_LARGER_THAN,
	OPCODE_TEST_STR_EMPTY,
	OPCODE_TEST_STR_NONEMPTY,
	OPCODE_TEST_STR_EQUALS,
	OPCODE_TEST_STR_UNEQUALS,
	OPCODE_TEST_NUM_EQUALS,
	OPCODE_TEST_NUM_UNEQUALS,
	OPCODE_TEST_NUM_GREATER,
	OPCODE_TEST_NUM_GREATER_EQUALS,
	OPCODE_TEST_NUM_LESSER,
	OPCODE_TEST_NUM_LESSER_EQUALS,
	OPCODE_TEST_IS_EXPR_FALSE,
	OPCODE_TEST_AND,
	OPCODE_TEST_OR,
	OPCODE_TEST_FILE_NEWER,
	OPCODE_TEST_FILE_OLDER,
	OPCODE_TERMINATE_HERE,
	OPCODE_SUBSHELL,
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
	VALUE_ARGS,
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
};

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
	FDesc fdesc;
};

struct StackValue
{
	enum ValueKind kind;
	union Value value;
};


struct Stack
{
	struct StackValue* stack[STACK_SIZE];
	int top;
};

struct Word* new_word(char* word, size_t len);
struct Name* new_name(char* name, size_t len);
struct Pattern* create_literal_node(char* literal);
struct Pattern* create_variable_node(char* variable_name);
struct Pattern* create_sequence_node(struct Pattern* children);
struct Pattern* create_alternation_node(struct Pattern* left, struct Pattern* right);
struct StackValue* create_word_value(struct Word* word);
struct StackValue* create_name_value(struct Name* name);
struct StackValue* create_parameter_value(struct Name* parameter);
struct StackValue* create_args_value(struct Arguments* args);
struct StackValue* create_special_param_value(enum SpecialParamKind specialParam);
struct StackValue* create_opcode_value(enum Opcode opcode);
struct StackValue* create_pos_param_value(PosParam posParam);
struct StackValue* create_fdesc_value(FDesc fdesc);
struct Stack* create_stack();
int is_stack_empty(struct Stack* stack);
int is_stack_full(struct Stack* stack);
int push(struct Stack* stack, struct StackValue* value);
struct StackValue* pop(struct Stack* stack);
struct StackValue* peek(struct Stack* stack);
void execute_stack(struct Stack** stack_pointer);


#endif /* machine.h */



