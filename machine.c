#include "machine.h"


struct Pattern* create_literal_node(char* literal) {
    struct Pattern* node = allocate_memory(sizeof(struct Pattern));
    node->type = PATTNODE_LITERAL;
    node->literal = strdup(literal);
    return node;
}


struct Pattern* create_variable_node(char* variable_name) {
    struct Pattern* node = allocate_memory(sizeof(struct Pattern));
    node->type = PATTNODE_VARIABLE;
    node->variable_name = strdup(variable_name);
    return node;
}


struct Pattern* create_sequence_node(struct Pattern* children) {
    struct Pattern* node = allocate_memory(sizeof(struct Pattern));
    node->type = PATTNODE_SEQUENCE;
    node->children = children;
    return node;
}


struct Pattern* create_alternation_node(struct Pattern* left, struct Pattern* right) {
    struct Pattern* node = allocate_memory(sizeof(struct Pattern));
    node->type = PATTNODE_ALTERNATION;
    node->children = allocate_memory(2 * sizeof(struct Pattern));
    node->children[0] = *left;
    node->children[1] = *right;
    return node;
}


struct Value* create_word_value(struct Word* word) {
    struct Value* newValue = allocate_memory(sizeof(struct Value));
    if (newValue != NULL) {
        newValue->kind = VALUE_WORD;
        newValue->value.word = word;
    }
    return newValue;
}


struct Value* createNameValue(struct Name* name) {
    struct Value* newValue = allocate_memory(sizeof(struct Value));
    if (newValue != NULL) {
        newValue->kind = VALUE_NAME;
        newValue->value.name = name;
    }
    return newValue;
}


struct Value* create_parameter_value(struct Name* parameter) {
    struct Value* newValue = allocate_memory(sizeof(struct Value));
    if (newValue != NULL) {
        newValue->kind = VALUE_PARAMETER;
        newValue->value.parameter = parameter;
    }
    return newValue;
}


struct Value* create_args_value(struct Arguments* args) {
    struct Value* newValue = allocate_memory(sizeof(struct Value));
    if (newValue != NULL) {
        newValue->kind = VALUE_ARGS;
        newValue->value.args = args;
    }
    return newValue;
}


struct Value* create_special_param_value(struct SpecialParamKind specialParam) {
    struct Value* newValue = allocate_memory(sizeof(struct Value));
    if (newValue != NULL) {
        newValue->kind = VALUE_SPECIAL_PARAM;
        newValue->value.special_param = specialParam;
    }
    return newValue;
}


struct Value* create_opcode_value(enum Opcode opcode) {
    struct Value* newValue = allocate_memory(sizeof(struct Value));
    if (newValue != NULL) {
        newValue->kind = VALUE_OPCODE;
        newValue->value.opcode = opcode;
    }
    return newValue;
}


struct Value* create_pos_param_value(PosParam posParam) {
    struct Value* newValue = allocate_memory(sizeof(struct Value));
    if (newValue != NULL) {
        newValue->kind = VALUE_POS_PARAM;
        newValue->value.pos_param = posParam;
    }
    return newValue;
}


struct Value* create_fdesc_value(Fdesc fdesc) {
    struct Value* newValue = allocate_memory(sizeof(struct Value));
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


int push(struct Stack* stack, struct Value* value) {
    if (is_stack_full(stack)) {
        return 0; 
    }

    stack->top++;
    stack->stack[stack->top] = value;
    return 1; 
}


struct Value* pop(struct Stack* stack) {
    if (is_stack_empty(stack)) {
        return NULL; 
    }

    struct Value* popped_value = stack->stack[stack->top];
    stack->top--;
    return popped_value;
}


struct Value* peek(struct Stack* stack) {
    if (is_stack_empty(stack)) {
        return NULL; 
    }

    return stack->stack[stack->top];
}
