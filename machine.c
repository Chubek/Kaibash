#ifndef STACK_SIZE
#define STACK_SIZE			(1 << 16)
#endif

#ifndef INSTRUCTION_STACK_SIZE
#define INSTRUCTION_STACK_SIZE		(1 << 16)
#endif

#ifndef WORD_SIZE
#define WORD_SIZE			(1 << 12)
#endif

#ifndef NAME_SIZE
#define NAME_SIZE			(1 << 8)
#endif

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

enum PatternNodeType {
	PATTNODE_LITERAL,
	PATTNODE_VARIABLE,
	PATTNODE_WILDCARD,
	PATTNODE_SEQUENCE,
	PATTNODE_ALTERNATION,
};


struct PatternAst {
    enum PatternNodeType type;
    union {
        char* literal;            
        char* variable_name;      
        struct PatternAst* children; 
    };
};


struct PatternAst* create_literal_node(char* literal) {
    struct PatternAst* node = allocate_memory(sizeof(struct PatternAst));
    node->type = PATTNODE_LITERAL;
    node->literal = strdup(literal);
    return node;
}


struct PatternAst* create_variable_node(char* variable_name) {
    struct PatternAst* node = allocate_memory(sizeof(struct PatternAst));
    node->type = PATTNODE_VARIABLE;
    node->variable_name = strdup(variable_name);
    return node;
}


struct PatternAst* create_sequence_node(struct PatternAst* children) {
    struct PatternAst* node = allocate_memory(sizeof(struct PatternAst));
    node->type = PATTNODE_SEQUENCE;
    node->children = children;
    return node;
}


struct PatternAst* create_alternation_node(struct PatternAst* left, struct PatternAst* right) {
    struct PatternAst* node = allocate_memory(sizeof(struct PatternAst));
    node->type = PATTNODE_ALTERNATION;
    node->children = allocate_memory(2 * sizeof(struct PatternAst));
    node->children[0] = *left;
    node->children[1] = *right;
    return node;
}

typedef int FDesc;
typedef int PosParam;

enum ValueType
{
	VALUE_WORD,
	VALUE_ARG,
	VALUE_NAME,
	VALUE_FDESC,
	VALUE_PATTERN,
	VALUE_PARAMETER,
	VALUE_SPECIAL_PARAMETER,
	VALUE_POSITIONAL_PARAMETER,
};

enum SpecialParamType
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
	struct PatternAst* pattern;
	struct Arguments* args;
	struct SpecialParamType special_param;
	PosParam pos_param;
	Fdesc fdesc;
};

struct Stack
{
	union Value* container[STACK_SIZE];
	size_t	top;
}

struct Instructions
{
	enum Instruction container[INSTRUCTION_STACK_SIZE];
	size_t top;
}

struct Machine
{
	struct Stack* stack;
	struct Instructions* instructions;
}

struct Machine* new_machine(void)
{
	return (struct Machine*) allocate_memory(sizeof(struct Machine));
}

void init_machine(struct Machine* machine)
{
	machine->stack = 
	   		(struct Stack*)	allocate_memory(sizeof(struct Stack));
	machine->instructions = 
			(struct Instructions*) allocate_memory(sizeof(struct Instructions));
}

void init_stack(struct Stack* stack)
{
	memset(stack, 0, sizeof(*stack));
}

void init_instructions(struct Instructions* instructions)
{
	memset(stack, 0, sizeof(*instructions));
}

void push_stack(struct Stack* stack, union Value* value)
{
	stack->container[stack->top] = allocate_memory(sizeof(*value));
	memmove(&stack->container[stack->top++], value, sizeof(*value));
}

union Value* pop_stack(struct Stack* stack)
{
	return stack->container[--stack->top];
}

void push_instruction(struct Instructions* instructions, enum Opcode opcode)
{
	instructions->container[instructions->top++] = opcode;
}

enum Opcode pop_instruction(struct Instructions* instructions)
{
	return instructions->container[--instructions->top];
}


