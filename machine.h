#ifndef MACHINE_H_
#define MACHINE_H_

#include <limits.h>

#ifndef STACK_SIZE
#define STACK_SIZE (1 << 16)
#endif

#ifndef WORD_SIZE
#define WORD_SIZE (1 << 12)
#endif

#ifndef NAME_SIZE
#define NAME_SIZE (1 << 8)
#endif

enum Opcode {
  OPCODE_SCRIPT_INIT,
  OPCODE_SCRIPT_END,

  OPCODE_TEST_TERM_NORMAL,
  OPCODE_TEST_TERM_NEGATE,
  OPCODE_TEST_TERM_BINARY,
  OPCODE_TEST_TERM_UNARY,
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

  OPCODE_STMT_ASSIGN,
  OPCODE_STMT_FUNCTION,
  
  OPCODE_CASE_PATTERN,
  OPCODE_CASE_ITEM,
  OPCODE_CASE_BEGIN,
  OPCODE_CASE_END,
  
  OPCODE_WORD_LIST_END,
  OPCODE_FOR_BEGIN,
  OPCODE_FOR_END,
  
  OPCODE_WHILE_BEGIN,
  OPCODE_WHILE_END,
  
  OPCODE_IF_BEGIN,
  OPCODE_IF_END,
  
  OPCODE_ELIF_BEGIN,
  OPCODE_ELIF_END,
  
  OPCODE_ELSE_BEGIN,
  OPCODE_ELSE_END,
 
  OPCODE_JUMP,
  OPCODE_JUMP_IF_TRUE,
  OPCODE_JUMP_IF_FALSE,
  
  OPCODE_DELIMITER,
  OPCODE_PATTERN,
  OPCODE_REPLACEMENT,
  
  OPCODE_ARITH_ADD,
  OPCODE_ARITH_SUB,
  OPCODE_ARITH_MUL,
  OPCODE_ARITH_DIV,
  
  OPCODE_SUB_EXPR,
  OPCODE_SUB_SHELL,
  OPCODE_TILDE_EXPN,
  OPCODE_REPLACE_ALL,
  OPCODE_REPLACE_FIRST,
  
  OPCODE_RM_LARGEST_PFX,
  OPCODE_RM_SMALLEST_PFX,
  OPCODE_RM_LARGEST_SFX,
  OPCODE_RM_SMALLEST_SFX,
  
  OPCODE_ALT_PLUS,
  OPCODE_ALT_DASH,
  OPCODE_ALT_EQUALS,
  OPCODE_ALT_QMARK,
  
  OPCODE_DELIM_COLON,
  OPCODE_NO_DELIM,
  OPCODE_VAR_SUB,
  
  OPCODE_INPUT_REDIR,
  OPCODE_OUTPUT_REDIR,
  OPCODE_APPEND_REDIR,
  OPCODE_DUPOUT_REDIR,
  OPCODE_DUPIN_REDIR,
  OPCODE_HERE_DOC,
  OPCODE_HERE_STR,
  
  OPCODE_SIMPLE_CMD,
  OPCODE_BANGED,
  OPCODE_PIPELINE,
  OPCODE_LIST_ANDIF,
  OPCODE_LIST_ANDOR,
  
  OPCODE_SWITCH,
  OPCODE_TRY,
  OPCODE_CATCH,
  OPCODE_FINALLY,

  OPCODE_NORMAL_EXEC,
  OPCODE_PARALLEL_EXEC,
  OPCODE_ASYNC_EXEC,

  OPCODE_ARRAY_CREATE,
  OPCODE_ARRAY_ACCESS,
  OPCODE_ASSOC_ARRAY_CREATE,
  OPCODE_JSON_PARSE,

  OPCODE_EXTENDED_VAR_EXPAND,
  OPCODE_SUBSTRING,

  OPCODE_NAMED_FUNCTION,
  OPCODE_MODULE_IMPORT,

  OPCODE_REFLECTION,
  OPCODE_MIXIN,

  OPCODE_STACK_TRACE,
  OPCODE_FAILWITH,

  OPCODE_OBJECT_MODEL,
  OPCODE_METHOD_INVOKE,

  OPCODE_API_CALL,
  OPCODE_DB_CONNECT,

  OPCODE_FUNCTIONAL_OP,
  OPCODE_DECLARATIVE_OP,

  OPCODE_ADDITIONAL_FUNCTIONS,
  OPCODE_REGEX_ENHANCEMENTS,

  OPCODE_JOB_DEPENDENCY,
  OPCODE_PRIORITY_SCHEDULING,

};

enum ValueKind {
  VALUE_WORD,
  VALUE_OPCODE,
  VALUE_ARGS,
  VALUE_NAME,
  VALUE_FDESC,
  VALUE_CHUNK,
  VALUE_COMMAND,
  VALUE_SPECIAL_PARAMETER,
  VALUE_POSITIONAL_PARAMETER,
};

enum SpecialParamKind {
  ARGC_NUM,
  ARGV,
  ENVIRON,
  HOME,
  PWD,
  SHELL,
  USER,
  UNKNOWN
};

typedef int FDesc;
typedef int PosParam;
typedef char *BytecodeChunk;

struct Word {
  char container[WORD_SIZE];
  size_t length;
};

struct Name {
  char container[NAME_SIZE];
  size_t length;
};

struct Arguments {
  char arguments[WORD_SIZE][ARG_MAX];
  size_t num_arg;
};

struct StackValue {
  enum ValueKind kind;
  union Value {
    struct Word *word;
    struct Name *name;
    struct SpecialParamKind special_param;
    enum Opcode opcode;
    PosParam pos_param;
    FDesc fdesc;
    BytecodeChunk chunk;
  };
};

struct Stack {
  struct StackValue *operand_stack;
  enum Opcode *inst_stack;
  size_t ostack_size, istack_size;
  int sp, ip;
};

struct Word *new_word(char *word, size_t len);
struct Name *new_name(char *name, size_t len);

struct StackValue *create_word_value(struct Word *word);
struct StackValue *create_bytecode_chunk(BytecodeChunk *chunk);
struct StackValue *create_name_value(struct Name *name);
struct StackValue *create_opcode_value(enum Opcode opcode);
struct StackValue *create_pos_param_value(PosParam posparam);
struct StackValue *
create_special_param_value(enum SpecialParamKind kind) struct StackValue
    *create_fdesc_value(FDesc fdesc);

bool value_valid_word(struct StackValue *value);
bool value_valid_name(struct StackValue *value);
bool value_valid_opcode(struct StackValue *value);
bool value_valid_pos_param(struct StackValue *value);
bool value_valid_special_param(struct StackValue *value);
bool value_valid_fdesc(struct StackValue *value);
enum SpecialParamKind get_sparam_kind(char c);

struct Stack *create_stack();
int is_stack_empty(struct Stack *stack);
int is_stack_full(struct Stack *stack);
int push_operand(struct Stack *stack, struct StackValue *value);
int push_inst(struct Stack *stack, enum Opcode inst);
struct StackValue *pop_operand(struct Stack *stack);
struct StackValue *peek_operand(struct Stack *stack);
enum Opcode pop_inst(struct Stack *stack);
enum Opcode peek_inst(struct Stack *stack);
void set_sp(struct Stack *stack, int sp);
void set_ip(struct Stack *stack, int ip);
int get_sp(struct Stack *stack);
int get_ip(struct Stack *stack);

#endif /* machine.h */
