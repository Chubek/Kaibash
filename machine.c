#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>

#include "tables.h"
#include "machine.h"



struct Pattern* create_literal_node(char* literal) {
    struct Pattern* node = allocate_memory(sizeof(struct Pattern));
    node->kind = PATTNODE_LITERAL;
    node->literal = strdup(literal);
    return node;
}


struct Pattern* create_variable_node(char* variable_name) {
    struct Pattern* node = allocate_memory(sizeof(struct Pattern));
    node->kind = PATTNODE_VARIABLE;
    node->variable_name = strdup(variable_name);
    return node;
}


struct Pattern* create_sequence_node(struct Pattern* children) {
    struct Pattern* node = allocate_memory(sizeof(struct Pattern));
    node->kind = PATTNODE_SEQUENCE;
    node->children = children;
    return node;
}


struct Pattern* create_alternation_node(struct Pattern* left, struct Pattern* right) {
    struct Pattern* node = allocate_memory(sizeof(struct Pattern));
    node->kind = PATTNODE_ALTERNATION;
    node->children = allocate_memory(2 * sizeof(struct Pattern));
    node->children[0] = *left;
    node->children[1] = *right;
    return node;
}


struct StackValue* create_word_value(struct Word* word) {
    struct StackValue* newValue = allocate_memory(sizeof(struct StackValue));
    if (newValue != NULL) {
        newValue->kind = VALUE_WORD;
        newValue->value.word = word;
    }
    return newValue;
}


struct StackValue* create_name_value(struct Name* name) {
    struct StackValue* newValue = allocate_memory(sizeof(struct StackValue));
    if (newValue != NULL) {
        newValue->kind = VALUE_NAME;
        newValue->value.name = name;
    }
    return newValue;
}


struct StackValue* create_parameter_value(struct Name* parameter) {
    struct StackValue* newValue = allocate_memory(sizeof(struct StackValue));
    if (newValue != NULL) {
        newValue->kind = VALUE_PARAMETER;
        newValue->value.parameter = parameter;
    }
    return newValue;
}


struct StackValue* create_args_value(struct Arguments* args) {
    struct StackValue* newValue = allocate_memory(sizeof(struct StackValue));
    if (newValue != NULL) {
        newValue->kind = VALUE_ARGS;
        newValue->value.args = args;
    }
    return newValue;
}


struct StackValue* create_special_param_value(enum SpecialParamKind specialParam) {
    struct StackValue* newValue = allocate_memory(sizeof(struct StackValue));
    if (newValue != NULL) {
        newValue->kind = VALUE_SPECIAL_PARAMETER;
        newValue->value.special_param = specialParam;
    }
    return newValue;
}


struct StackValue* create_opcode_value(enum Opcode opcode) {
    struct StackValue* newValue = allocate_memory(sizeof(struct StackValue));
    if (newValue != NULL) {
        newValue->kind = VALUE_OPCODE;
        newValue->value.opcode = opcode;
    }
    return newValue;
}


struct StackValue* create_pos_param_value(PosParam posParam) {
    struct StackValue* newValue = allocate_memory(sizeof(struct StackValue));
    if (newValue != NULL) {
        newValue->kind = VALUE_POSITIONAL_PARAMETER;
        newValue->value.pos_param = posParam;
    }
    return newValue;
}


struct StackValue* create_fdesc_value(FDesc fdesc) {
    struct StackValue* newValue = allocate_memory(sizeof(struct StackValue));
    if (newValue != NULL) {
        newValue->kind = VALUE_FDESC;
        newValue->value.fdesc = fdesc;
    }
    return newValue;
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
