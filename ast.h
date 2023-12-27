#ifndef AST_H_
#define AST_H_



#include <stdio.h>
#include <stdlib.h>

#include "tables.h"

typedef enum {
    NODE_PROGRAM,
    NODE_COMMAND,
    NODE_SIMPLE_COMMAND,
    NODE_SIMPLE_COMMAND_REDIR,
    NODE_REDIR,
    NODE_INPUT_REDIR,
    NODE_OUTPUT_REDIR,
    NODE_APPEND_OUTPUT_REDIR,
    NODE_DUPLICATE_OUTPUT_REDIR,
    NODE_DUPLICATE_INPUT_REDIR,
    NODE_HERE_DOCUMENT,
    NODE_HERE_STRING,
    NODE_WORD,
    NODE_WORD_LIST,
    NODE_QUOTED_STRING,
    NODE_STRING,
    NODE_BACKTICK_EXPANSION,
    NODE_SUBSTITUTION,
    NODE_VARIABLE_SUBSTITUTION,
    NODE_COMPLEX_VARIABLE,
    NODE_ALTERNATIVE,
    NODE_REMOVE_SMALLEST_SUFFIX,
    NODE_REMOVE_LARGEST_SUFFIX,
    NODE_REMOVE_SMALLEST_PREFIX,
    NODE_REMOVE_LARGEST_PREFIX,
    NODE_REPLACE_FIRST,
    NODE_REPLACE_ALL,
    NODE_COMMAND_SUBSTITUTION,
    NODE_ARITHMETIC_SUBSTITUTION,
    NODE_ARITHMETIC_EXPRESSION,
    NODE_ADDITION,
    NODE_SUBTRACTION,
    NODE_TERM,
    NODE_MULTIPLICATION,
    NODE_DIVISION,
    NODE_FACTOR,
    NODE_PATTERN,
    NODE_REPLACEMENT,
    NODE_NAME,
    NODE_FD,
    NODE_DELIMITER,
    NODE_IF_STATEMENT,
    NODE_IF_PREFIX,
    NODE_WHILE_LOOP,
    NODE_WHILE_LOOP_PREFIX,
    NODE_FOR_LOOP,
    NODE_FOR_LOOP_PREFIX,
    NODE_CASE_STATEMENT,
    NODE_CASE_PATTERN_MULTIPLE,
    NODE_CASE_PATTERN_SINGLE,
    NODE_PATTERN_LIST,
    NODE_FUNCTION_DEFINITION,
    NODE_VARIABLE_ASSIGNMENT,
    NODE_COMMENT,
    NODE_COMPOUND_COMMAND,
    NODE_TEST_EXPR,
    NODE_TEST_JUNCTION,
    NODE_TEST_TERM,
    NODE_NEGATED_TEST_TERM,
    NODE_NORMAL_TEST_TERM,
    NODE_UNARY_OPERATOR,
    NODE_BINARY_OPERATOR,
    NODE_ANY_CHAR,
    NODE_EOF,
} NodeType;


typedef struct ASTNode {
    NodeType type;
    char* value;  
    struct ASTNode* children;
    struct ASTNode* next;
} ASTNode;


ASTNode* create_node(NodeType type, char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value;
    node->children = NULL;
    node->next = NULL;
    return node;
}


void append_child(ASTNode* parent, ASTNode* child) {
    if (!parent->children) {
        parent->children = child;
    } else {
        ASTNode* lastChild = parent->children;
        while (lastChild->next) {
            lastChild = lastChild->next;
        }
        lastChild->next = child;
    }
}


#endif /* ast.h */
