#ifndef ABSYN_H_
#define ABSYN_H_

#include <stdio.h>

#include "tables.h"


enum AbsynNodeKind {
    PROGRAM_NODE,
    COMMAND_NODE,
    SIMPLE_COMMAND_NODE,
    WORD_NODE,
    STRING_NODE,
    SUBSTITUTION_NODE,
    VARIABLE_SUBSTITUTION_NODE,
    COMPLEX_VARIABLE_NODE,
    REDIRECTION_NODE,
    INPUT_REDIRECTION_NODE,
    OUTPUT_REDIRECTION_NODE,
    APPEND_OUTPUT_REDIRECTION_NODE,
    DUPLICATE_OUTPUT_REDIRECTION_NODE,
    DUPLICATE_INPUT_REDIRECTION_NODE,
    HEREDOCUMENT_NODE,
    HERESTRING_NODE,
    PIPELINE_NODE,
    IF_STATEMENT_NODE,
    ELIF_STATEMENT_NODE,
    WHILE_LOOP_NODE,
    FOR_LOOP_NODE,
    CASE_STATEMENT_NODE,
    FUNCTION_DEFINITION_NODE,
    VARIABLE_ASSIGNMENT_NODE,
    COMMENT_NODE,
    COMPOUND_COMMAND_NODE,
    TEST_COMMAND_NODE,
    TEST_EXPRESSION_NODE,
    TEST_TERM_NODE,
    TEST_EXPRESSION_TAIL_NODE,
    TEST_EXP_CONJUNCTION_NODE,
    TEST_EXP_DISJUNCTION_NODE,
    UNARY_TEST_NODE,
    UNARY_OPERATOR_NODE,
    TEST_OPERATOR_NODE,
    ANY_CHAR_EXCEPT_SPACE_NODE,
    ANY_CHARACTER_NODE,
    EOF_NODE
};


struct AbsynNode {
    enum AbsynNodeKind kind;
    char* value; 
    union {
        struct AbsynNode* next; 
        enum AbsynNodeKind redirection_kind; 
    };
};


static inline struct AbsynNode* mk_absyn_node(enum AbsynNodeKind kind, const char* value) {
    struct AbsynNode* new_node = (struct AbsynNode*)allocate_memory(MEMTAG_ABSYN, sizeof(struct AbsynNode));

    new_node->kind = kind;
    new_node->value = duplicate_string(value);
    new_node->next = NULL;

    return new_node;
}


static inline void add_absyn_node(struct AbsynNode** head, struct AbsynNode* new_node) {
    if (*head == NULL) {
        *head = new_node;
    } else {
        struct AbsynNode* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}


#endif /* absyn.h */
