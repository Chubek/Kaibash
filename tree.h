#ifndef TREE_H_
#define TREE_H_

enum NodeKind {
  PROGRAM,
  COMMAND_LIST,
  SEPARATOR,
  COMMAND,
  SIMPLE_COMMAND,
  REDIRECTION,
  DUPLICATE,
  TEST_COMMAND,
  TEST_EXPRESSION,
  TEST_TERM,
  UNARY_TEST,
  UNARY_OPERATOR,
  TEST_OPERATOR,
  IF_STATEMENT,
  WHILE_LOOP,
  FOR_LOOP,
  WORD_LIST,
  CASE_STATEMENT,
  PATTERN_LIST,
  FUNCTION_DEFINITION,
  VARIABLE_ASSIGNMENT,
  LITERAL,
  COMPOUND_COMMAND,
  COMMAND_SUBSTITUTION,
  VARIABLE,
  STRING,
  DOUBLE_QUOTED_STRING,
  DOUBLE_QUOTED_STRING_CONTENT,
  SINGLE_QUOTED_STRING
};

struct AbsynNode {
  enum NodeKind kind;
  struct AbsynNode *left;
  struct AbsynNode *right;
  char *value;
};

struct AbsynNode *create_node(enum NodeKind kind);

struct AbsynNode *create_program_node(struct AbsynNode *command_list);
struct AbsynNode *create_command_list_node(struct AbsynNode *command,
                                           struct AbsynNode *separator,
                                           struct AbsynNode *next_command_list);
struct AbsynNode *create_separator_node(enum NodeKind kind);
struct AbsynNode *create_command_node(struct AbsynNode *command,
                                      struct AbsynNode *next_command);
struct AbsynNode *create_simple_command_node(struct AbsynNode *word_list,
                                             struct AbsynNode *redirection);
struct AbsynNode *create_redirection_node(enum NodeKind kind,
                                          struct AbsynNode *word);
struct AbsynNode *create_duplicate_node(struct AbsynNode *word1,
                                        struct AbsynNode *word2);
struct AbsynNode *create_test_command_node(struct AbsynNode *test_expression);
struct AbsynNode *create_test_expression_node(struct AbsynNode *term1,
                                              enum NodeKind operator,
                                              struct AbsynNode * term2);
struct AbsynNode *create_test_term_node(enum NodeKind kind,
                                        struct AbsynNode *content);
struct AbsynNode *create_unary_test_node(enum NodeKind kind,
                                         struct AbsynNode *content);
struct AbsynNode *create_unary_operator_node(enum NodeKind kind);
struct AbsynNode *create_test_operator_node(enum NodeKind kind);
struct AbsynNode *create_if_statement_node(struct AbsynNode *condition,
                                           struct AbsynNode *if_block,
                                           struct AbsynNode *else_block);
struct AbsynNode *create_while_loop_node(struct AbsynNode *condition,
                                         struct AbsynNode *loop_block);
struct AbsynNode *create_for_loop_node(struct AbsynNode *variable,
                                       struct AbsynNode *word_list,
                                       struct AbsynNode *loop_block);
struct AbsynNode *create_word_list_node(struct AbsynNode *word,
                                        struct AbsynNode *next_word_list);
struct AbsynNode *
create_case_statement_node(struct AbsynNode *word,
                           struct AbsynNode *pattern_list,
                           struct AbsynNode *compound_command);
struct AbsynNode *create_pattern_list_node(struct AbsynNode *pattern,
                                           struct AbsynNode *next_pattern_list);
struct AbsynNode *
create_function_definition_node(struct AbsynNode *name,
                                struct AbsynNode *compound_command);
struct AbsynNode *create_variable_assignment_node(struct AbsynNode *name,
                                                  struct AbsynNode *literal);
struct AbsynNode *create_literal_node(enum NodeKind kind, char *value);
struct AbsynNode *create_compound_command_node(struct AbsynNode *command_list);
struct AbsynNode *
create_command_substitution_node(struct AbsynNode *compound_command);
struct AbsynNode *create_variable_node(struct AbsynNode *name);
struct AbsynNode *create_string_node(enum NodeKind kind,
                                     struct AbsynNode *content);
struct AbsynNode *create_double_quoted_string_node(struct AbsynNode *content);
struct AbsynNode *
create_double_quoted_string_content_node(struct AbsynNode *content);
struct AbsynNode *create_single_quoted_string_node(struct AbsynNode *word_list);

#endif /* tree.h */
