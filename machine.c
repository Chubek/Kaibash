#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/types.h>

#include "tables.h"
#include "machine.h"

extern void print_location_info(void);

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

struct Word* new_word(char* word, size_t length)
{
	if (length >= WORD_SIZE)
	{
		fprintf(stderr, "Error: word is larger than maximum allowed size (of %lu)\n", WORD_SIZE);
		print_location_info();
		exit(EXIT_FAILURE);
	}

	struct Word* word = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct Word));

	if (strncpy(&word->container[0], word) < 0)
	{
		perror("strncpy");
		exit(EXIT_FAILURE);
	}

	word->length = length;
}

struct Name* new_name(char* word, size_t length)
{
	if (length >= NAME_SIZE)
	{
		fprintf(stderr, "Error: name is larger than maximum allowed size (of %lu)\n", WORD_SIZE);
		print_location_info();
		exit(EXIT_FAILURE);
	}

	struct Name* name = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct Name));

	if (strncpy(&word->container[0], word) < 0)
	{
		perror("strncpy");
		exit(EXIT_FAILURE);
	}

	word->length = length;
}

struct StackValue* create_word_value(struct Word* word) {
    struct StackValue* newval = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct StackValue));
    if (newval != NULL) {
        newval->kind = VALUE_WORD;
        newval->value.word = word;
    }
    return newval;
}


struct StackValue* create_name_value(struct Name* name) {
    struct StackValue* newval = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct StackValue));
    if (newval != NULL) {
        newval->kind = VALUE_NAME;
        newval->value.name = name;
    }
    return newval;
}


struct StackValue* create_parameter_value(struct Name* parameter) {
    struct StackValue* newval = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct StackValue));
    if (newval != NULL) {
        newval->kind = VALUE_PARAMETER;
        newval->value.parameter = parameter;
    }
    return newval;
}

struct StackValue* create_special_param_value(enum SpecialParamKind specialParam) {
    struct StackValue* newval = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct StackValue));
    if (newval != NULL) {
        newval->kind = VALUE_SPECIAL_PARAMETER;
        newval->value.special_param = specialParam;
    }
    return newval;
}


struct StackValue* create_opcode_value(enum Opcode opcode) {
    struct StackValue* newval = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct StackValue));
    if (newval != NULL) {
        newval->kind = VALUE_OPCODE;
        newval->value.opcode = opcode;
    }
    return newval;
}


struct StackValue* create_pos_param_value(PosParam pos_param) {
    struct StackValue* newval = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct StackValue));
    if (newval != NULL) {
        newval->kind = VALUE_POSITIONAL_PARAMETER;
        newval->value.pos_param = pos_param;
    }
    return newval;
}


struct StackValue* create_fdesc_value(FDesc fdesc) {
    struct StackValue* newval = allocate_memory(MEMTAG_STACK_PARAM, sizeof(struct StackValue));
    if (newval != NULL) {
        newval->kind = VALUE_FDESC;
        newval->value.fdesc = fdesc;
    }
    return newval;
}


bool is_value_word(const Value* value) {
    return value != NULL && value->kind == VALUE_WORD;
}

bool is_value_name(const Value* value) {
    return value != NULL && value->kind == VALUE_NAME;
}

bool is_value_opcode(const Value* value) {
    return value != NULL && value->kind == VALUE_OPCODE;
}

bool is_value_pos_param(const Value* value) {
    return value != NULL && value->kind == VALUE_POSITIONAL_PARAMETER;
}

bool is_value_special_param(const Value* value) {
    return value != NULL && value->kind == VALUE_SPECIAL_PARAMETER;
}

bool is_value_fdesc(const Value* value) {
    return value != NULL && value->kind == VALUE_FDESC;
}


struct Stack* create_stack() {
    struct Stack* newStack = malloc(sizeof(struct Stack));
    if (newStack != NULL) {
        newStack->top = -1; 
    }
    return newStack;
}


int is_stack_empty(struct Stack* stack) {
    return stack->top == -1;
}


int is_stack_full(struct Stack* stack) {
    return stack->top == STACK_SIZE - 1;
}


int push(struct Stack* stack, struct StackValue* value) {
    if (is_stack_full(stack)) {
        return 0; 
    }

    stack->top++;
    stack->stack[stack->top] = value;
    return 1; 
}


struct StackValue* pop(struct Stack* stack) {
    if (is_stack_empty(stack)) {
        return NULL; 
    }

    struct StackValue* popped_value = stack->stack[stack->top];
    stack->top--;
    return popped_value;
}


struct StackValue* peek(struct Stack* stack) {
    if (is_stack_empty(stack)) {
        return NULL; 
    }

    return stack->stack[stack->top];
}
