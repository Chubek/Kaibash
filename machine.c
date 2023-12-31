#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "machine.h"
#include "tables.h"

enum SpecialParamKind get_sparam_kind(char c) {
  switch (c) {
  case '#':
    return ARGC_NUM;
  case '@':
    return ARGV;
  case '*':
    return ENVIRON;
  case '~':
    return HOME;
  case '/':
    return PWD;
  case '-':
    return SHELL;
  case '$':
    return USER;
  default:
    return UNKNOWN;
  }
}

struct Word *new_word(char *word, size_t length) {
  if (length >= WORD_SIZE) {
    fprintf(stderr,
            "Error: word is larger than maximum allowed size (of %lu)\n",
            WORD_SIZE);
    print_location_info();
    exit(EXIT_FAILURE);
  }

  struct Word *word = allocate_memory(MEMTAG_STACK_VAL, sizeof(struct Word));

  if (strncpy(&word->container[0], word) < 0) {
    perror("strncpy");
    exit(EXIT_FAILURE);
  }

  word->length = length;
}

struct Name *new_name(char *word, size_t length) {
  if (length >= NAME_SIZE) {
    fprintf(stderr,
            "Error: name is larger than maximum allowed size (of %lu)\n",
            WORD_SIZE);
    print_location_info();
    exit(EXIT_FAILURE);
  }

  struct Name *name = allocate_memory(MEMTAG_STACK_VAL, sizeof(struct Name));

  if (strncpy(&word->container[0], word) < 0) {
    perror("strncpy");
    exit(EXIT_FAILURE);
  }

  word->length = length;
}

struct StackValue *create_word_value(struct Word *word) {
  struct StackValue *newval =
      allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_WORD;
    newval->word = word;
  }
  return newval;
}

struct StackValue *create_name_value(struct Name *name) {
  struct StackValue *newval =
      allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_NAME;
    newval->name = name;
  }
  return newval;
}

struct StackValue *create_bytecode_chunk(BytecodeChunk chunk) {
  struct StackValue *newval =
      allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_CHUNK;
    newval->chunk = chunk;
  }
  return newval;
}

struct StackValue *create_parameter_value(struct Name *parameter) {
  struct StackValue *newval =
      allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_PARAMETER;
    newval->parameter = parameter;
  }
  return newval;
}

struct StackValue *
create_special_param_value(enum SpecialParamKind specialParam) {
  struct StackValue *newval =
      allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_SPECIAL_PARAMETER;
    newval->special_param = specialParam;
  }
  return newval;
}

struct StackValue *create_opcode_value(enum Opcode opcode) {
  struct StackValue *newval =
      allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_OPCODE;
    newval->opcode = opcode;
  }
  return newval;
}

struct StackValue *create_regex_pattern_value(RegexPattern *pattern) {
  struct StackValue *newval = 
	  allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_REGEX_PATTER;
    newval->re_pattern = pattern;
  }
  return newval;
}

struct StackValue *create_pos_param_value(PosParam pos_param) {
  struct StackValue *newval =
      allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_POSITIONAL_PARAMETER;
    newval->pos_param = pos_param;
  }
  return newval;
}

struct StackValue *create_fdesc_value(FDesc fdesc) {
  struct StackValue *newval =
      allocate_memory(MEMTAG_STACK_VAL, sizeof(struct StackValue));
  if (newval != NULL) {
    newval->kind = VALUE_FDESC;
    newval->fdesc = fdesc;
  }
  return newval;
}

bool value_valid_word(struct StackValue *value) {
  return value != NULL && value->kind == VALUE_WORD;
}

bool value_valid_name(struct StackValue *value) {
  return value != NULL && value->kind == VALUE_NAME;
}

bool value_valid_opcode(struct StackValue *value) {
  return value != NULL && value->kind == VALUE_OPCODE;
}

bool value_valid_pos_param(struct StackValue *value) {
  return value != NULL && value->kind == VALUE_POSITIONAL_PARAMETER;
}

bool value_valid_special_param(struct StackValue *value) {
  return value != NULL && value->kind == VALUE_SPECIAL_PARAMETER;
}

bool value_valid_fdesc(struct StackValue *value) {
  return value != NULL && value->kind == VALUE_FDESC;
}

bool value_valid_regex_pattern(struct StackValue *value) {
  return value != NULL && value->kind == VALUE_REGEX_PATTERN;
}

bool value_valid_chunk(struct StackValue *value) {
  return value != NULL && value->kind = VALUE_CHUNK;
}

struct Stack *create_stack(size_t ostack_size, size_t istack_size) {
  struct Stack *stack = (struct Stack *)allocate_memory(sizeof(struct Stack));
  if (stack == NULL) {
    perror("Error creating stack");
    exit(EXIT_FAILURE);
  }

  stack->operand_stack = (struct StackValue *)allocate_memory(
      sizeof(struct StackValue) * ostack_size);
  if (stack->operand_stack == NULL) {
    perror("Error creating operand stack");
    exit(EXIT_FAILURE);
  }

  stack->inst_stack =
      (enum Opcode *)allocate_memory(sizeof(enum Opcode) * istack_size);
  if (stack->inst_stack == NULL) {
    perror("Error creating instruction stack");
    exit(EXIT_FAILURE);
  }

  stack->ostack_size = ostack_size;
  stack->istack_size = istack_size;
  stack->sp = -1;
  stack->ip = -1;

  return stack;
}

void destroy_stack(struct Stack *stack) {
  free(stack->operand_stack);
  free(stack->inst_stack);
  free(stack);
}

int is_stack_empty(struct Stack *stack) { return stack->sp == -1; }

int is_stack_full(struct Stack *stack) {
  return stack->sp == (int)(stack->ostack_size - 1);
}

int push_operand(struct Stack *stack, struct StackValue *value) {
  if (is_stack_full(stack)) {
    fprintf(stderr, "Operand stack overflow\n");
    return -1;
  }

  stack->operand_stack[++stack->sp] = *value;
  return 0;
}

int push_inst(struct Stack *stack, enum Opcode inst) {
  if (is_stack_full(stack)) {
    fprintf(stderr, "Instruction stack overflow\n");
    return -1;
  }

  stack->inst_stack[++stack->ip] = inst;
  return 0;
}

struct StackValue *pop_operand(struct Stack *stack) {
  if (is_stack_empty(stack)) {
    fprintf(stderr, "Operand stack underflow\n");
    return NULL;
  }

  return &stack->operand_stack[stack->sp--];
}

struct StackValue *peek_operand(struct Stack *stack) {
  if (is_stack_empty(stack)) {
    fprintf(stderr, "Operand stack is empty\n");
    return NULL;
  }

  return &stack->operand_stack[stack->sp];
}

enum Opcode pop_inst(struct Stack *stack) {
  if (is_stack_empty(stack)) {
    fprintf(stderr, "Instruction stack underflow\n");
    return OPCODE_TERM; // Return a default value for now
  }

  return stack->inst_stack[stack->ip--];
}

enum Opcode peek_inst(struct Stack *stack) {
  if (is_stack_empty(stack)) {
    fprintf(stderr, "Instruction stack is empty\n");
    return OPCODE_TERM; // Return a default value for now
  }

  return stack->inst_stack[stack->ip];
}

void set_sp(struct Stack *stack, int sp) { stack->sp = sp; }

void set_ip(struct Stack *stack, int ip) { stack->ip = ip; }

int get_sp(struct Stack *stack) { return stack->sp; }

int get_ip(struct Stack *stack) { return stack->ip; }
