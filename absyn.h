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
};

struct AbsynNode {
  enum AbsynNodeKind kind;
  char *value;
  union {
    struct AbsynNode *next;
    enum AbsynNodeKind redirection_kind;
  };
};

static inline struct AbsynNode *mk_absyn_node(enum AbsynNodeKind kind,
                                              const char *value) {
  struct AbsynNode *new_node = (struct AbsynNode *)allocate_memory(
      MEMTAG_ABSYN, sizeof(struct AbsynNode));

  new_node->kind = kind;
  new_node->value = duplicate_string(value);
  new_node->next = NULL;

  return new_node;
}

static inline void add_absyn_node(struct AbsynNode **head,
                                  struct AbsynNode *new_node) {
  if (*head == NULL) {
    *head = new_node;
  } else {
    struct AbsynNode *current = *head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
  }
}

static inline struct AbsynNode *Program(struct AbsynNode *commandList,
                                        struct AbsynNode *eof) {
  struct AbsynNode *programNode = mk_absyn_node(PROGRAM_NODE, "PROGRAM");
  add_absyn_node(&programNode->next, commandList);
  add_absyn_node(&programNode->next, eof);
  return programNode;
}

static inline struct AbsynNode *
Command_SimpleCommand(struct AbsynNode *simpleCommandNode) {
  struct AbsynNode *commandNode = mk_absyn_node(COMMAND_NODE, "COMMAND");
  add_absyn_node(&commandNode->next, simpleCommandNode);
  return commandNode;
}

static inline struct AbsynNode *
Command_Pipeline(struct AbsynNode *pipelineNode) {
  struct AbsynNode *commandNode = mk_absyn_node(COMMAND_NODE, "COMMAND");
  add_absyn_node(&commandNode->next, pipelineNode);
  return commandNode;
}

static inline struct AbsynNode *
Command_IfStatement(struct AbsynNode *ifStatementNode) {
  struct AbsynNode *commandNode = mk_absyn_node(COMMAND_NODE, "COMMAND");
  add_absyn_node(&commandNode->next, ifStatementNode);
  return commandNode;
}

static inline struct AbsynNode *
Command_TestCommand(struct AbsynNode *testCommandNode) {
  struct AbsynNode *commandNode = mk_absyn_node(COMMAND_NODE, "COMMAND");
  add_absyn_node(&commandNode->next, testCommandNode);
  return commandNode;
}

static inline struct AbsynNode *
Command_CompoundCommand(struct AbsynNode *compoundCommandNode) {
  struct AbsynNode *commandNode = mk_absyn_node(COMMAND_NODE, "COMMAND");
  add_absyn_node(&commandNode->next, compoundCommandNode);
  return commandNode;
}

static inline struct AbsynNode *SimpleCommand(struct AbsynNode *words,
                                              struct AbsynNode *redirections) {
  struct AbsynNode *simpleCommandNode =
      mk_absyn_node(SIMPLE_COMMAND_NODE, "SIMPLE_COMMAND");
  add_absyn_node(&simpleCommandNode->next, words);
  add_absyn_node(&simpleCommandNode->next, redirections);
  return simpleCommandNode;
}

static inline struct AbsynNode *Pipeline(struct AbsynNode *command,
                                         struct AbsynNode *pipeline) {
  struct AbsynNode *pipelineNode = mk_absyn_node(PIPELINE_NODE, "PIPELINE");
  add_absyn_node(&pipelineNode->next, command);
  add_absyn_node(&pipelineNode->next, pipeline);
  return pipelineNode;
}

static inline struct AbsynNode *IfStatement(struct AbsynNode *condition,
                                            struct AbsynNode *trueBranch,
                                            struct AbsynNode *falseBranch) {
  struct AbsynNode *ifStatementNode =
      mk_absyn_node(IF_STATEMENT_NODE, "IF_STATEMENT");
  add_absyn_node(&ifStatementNode->next, condition);
  add_absyn_node(&ifStatementNode->next, trueBranch);
  if (falseBranch != NULL) {
    add_absyn_node(&ifStatementNode->next, falseBranch);
  }
  return ifStatementNode;
}

static inline struct AbsynNode *ElifStatement(struct AbsynNode *condition,
                                              struct AbsynNode *trueBranch) {
  struct AbsynNode *elifStatementNode =
      mk_absyn_node(ELIF_STATEMENT_NODE, "ELIF_STATEMENT");
  add_absyn_node(&elifStatementNode->next, condition);
  add_absyn_node(&elifStatementNode->next, trueBranch);
  return elifStatementNode;
}

static inline struct AbsynNode *
ElifStatement(struct AbsynNode *compoundCommand,
              struct AbsynNode *compoundCommand2) {

  struct AbsynNode *elifStatementNode =
      mk_absyn_node(ELIF_STATEMENT_NODE, "ELIF_STATEMENT");
  add_absyn_node(&elifStatementNode->next, compoundCommand1);
  add_absyn_node(&elifStatementNode->next, compoundCommand2);
  return elifStatementNode;
}

static inline struct AbsynNode *WhileLoop(struct AbsynNode *compoundCommand1,
                                          struct AbsynNode *compoundCommand2) {

  struct AbsynNode *whileLoopNode =
      mk_absyn_node(WHILE_LOOP_NODE, "WHILE_LOOP");
  add_absyn_node(&whileLoopNode->next, compoundCommand1);
  add_absyn_node(&whileLoopNode->next, compoundCommand2);
  return whileLoopNode;
}

static inline struct AbsynNode *ForLoop(struct AbsynNode *name,
                                        struct AbsynNode *wordList,
                                        struct AbsynNode *compoundCommand, ) {

  struct AbsynNode *forLoopNode = mk_absyn_node(FOR_LOOP_NODE, "FOR_LOOP");
  add_absyn_node(&forLoopNode->next, name);
  add_absyn_node(&forLoopNode->next, wordList);
  add_absyn_node(&forLoopNode->next, compoundCommand);
  return forLoopNode;
}

static inline struct AbsynNode *CaseStatement(struct AbsynNode *word,
                                              struct AbsynNode *patterns) {

  struct AbsynNode *caseStatementNode =
      mk_absyn_node(CASE_STATEMENT_NODE, "CASE_STATEMENT");
  add_absyn_node(&caseStatementNode->next, word);
  add_absyn_node(&caseStatementNode->next, patterns);
  return caseStatementNode;
}

static inline struct AbsynNode *
FunctionDefinition(struct AbsynNode *name, struct AbsynNode *compoundCommand) {

  struct AbsynNode *functionDefinitionNode =
      mk_absyn_node(FUNCTION_DEFINITION_NODE, "FUNCTION_DEFINITION");
  add_absyn_node(&functionDefinitionNode->next, name);
  add_absyn_node(&functionDefinitionNode->next, compoundCommand);
  return functionDefinitionNode;
}

static inline struct AbsynNode *VariableAssignment(struct AbsynNode *name,
                                                   struct AbsynNode *word) {

  struct AbsynNode *variableAssignmentNode =
      mk_absyn_node(VARIABLE_ASSIGNMENT_NODE, "VARIABLE_ASSIGNMENT");
  add_absyn_node(&variableAssignmentNode->next, name);
  add_absyn_node(&variableAssignmentNode->next, word);
  return variableAssignmentNode;
}

static inline struct AbsynNode *CompoundCommand(struct AbsynNode *commands) {

  struct AbsynNode *compoundCommandNode =
      mk_absyn_node(COMPOUND_COMMAND_NODE, "COMPOUND_COMMAND");
  add_absyn_node(&compoundCommandNode->next, commands);
  return compoundCommandNode;
}

static inline struct AbsynNode *TestCommand(struct AbsynNode *leftBracket,
                                            struct AbsynNode *testExpression,
                                            struct AbsynNode *rightBracket) {

  struct AbsynNode *testCommandNode =
      mk_absyn_node(TEST_COMMAND_NODE, "TEST_COMMAND");
  add_absyn_node(&testCommandNode->next, testExpression);
  return testCommandNode;
}

static inline struct AbsynNode *Substitution(struct AbsynNode *substitution) {

  struct AbsynNode *substitutionNode =
      mk_absyn_node(SUBSTITUTION_NODE, "SUBSTITUTION");
  add_absyn_node(&substitutionNode->next, substitution);
  return substitutionNode;
}

static inline struct AbsynNode *
VariableSubstitution(struct AbsynNode *name,
                     struct AbsynNode *complexVariable) {

  struct AbsynNode *variableSubstitutionNode =
      mk_absyn_node(VARIABLE_SUBSTITUTION_NODE, "VARIABLE_SUBSTITUTION");
  add_absyn_node(&variableSubstitutionNode->next, name);
  add_absyn_node(&variableSubstitutionNode->next, complexVariable);
  return variableSubstitutionNode;
}

static inline struct AbsynNode *
ComplexVariable(struct AbsynNode *variableName) {

  struct AbsynNode *complexVariableNode =
      mk_absyn_node(COMPLEX_VARIABLE_NODE, "COMPLEX_VARIABLE");
  add_absyn_node(&complexVariableNode->next, variableName);
  return complexVariableNode;
}

static inline struct AbsynNode *VariableName(struct AbsynNode *name,
                                             struct AbsynNode *alternative,
                                             struct AbsynNode *delimiter) {

  struct AbsynNode *variableNameNode =
      mk_absyn_node(VARIABLE_NAME_NODE, "VARIABLE_NAME");
  add_absyn_node(&variableNameNode->next, name);
  add_absyn_node(&variableNameNode->next, alternative);
  add_absyn_node(&variableNameNode->next, delimiter);
  return variableNameNode;
}

static inline struct AbsynNode *TestCommand(struct AbsynNode *testExpression) {

  struct AbsynNode *testCommandNode =
      mk_absyn_node(TEST_COMMAND_NODE, "TEST_COMMAND");
  add_absyn_node(&testCommandNode->next, testExpression);
  return testCommandNode;
}

static inline struct AbsynNode *
TestExpression(struct AbsynNode *testTerm,
               struct AbsynNode *testExpressionTail) {

  struct AbsynNode *testExpressionNode =
      mk_absyn_node(TEST_EXPRESSION_NODE, "TEST_EXPRESSION");
  add_absyn_node(&testExpressionNode->next, testTerm);
  add_absyn_node(&testExpressionNode->next, testExpressionTail);
  return testExpressionNode;
}

static inline struct AbsynNode *
TestExpressionTail(struct AbsynNode *testOperator, struct AbsynNode *testTerm) {

  struct AbsynNode *testExpressionTailNode =
      mk_absyn_node(TEST_EXPRESSION_TAIL_NODE, "TEST_EXPRESSION_TAIL");
  add_absyn_node(&testExpressionTailNode->next, testOperator);
  add_absyn_node(&testExpressionTailNode->next, testTerm);
  return testExpressionTailNode;
}

static inline struct AbsynNode *TestTerm(struct AbsynNode *unaryTest,
                                         struct AbsynNode *testExpression,
                                         struct AbsynNode *testExpConjunction,
                                         struct AbsynNode *testExpDisjunction,
                                         struct AbsynNode *subExpression) {

  struct AbsynNode *testTermNode = mk_absyn_node(TEST_TERM_NODE, "TEST_TERM");
  add_absyn_node(&testTermNode->next, unaryTest);
  add_absyn_node(&testTermNode->next, testExpression);
  add_absyn_node(&testTermNode->next, testExpConjunction);
  add_absyn_node(&testTermNode->next, testExpDisjunction);
  add_absyn_node(&testTermNode->next, subExpression);
  return testTermNode;
}

static inline struct AbsynNode *UnaryTest(struct AbsynNode *unaryOperator,
                                          struct AbsynNode *word,
                                          struct AbsynNode *testExpression) {

  struct AbsynNode *unaryTestNode =
      mk_absyn_node(UNARY_TEST_NODE, "UNARY_TEST");
  add_absyn_node(&unaryTestNode->next, unaryOperator);
  add_absyn_node(&unaryTestNode->next, word);
  add_absyn_node(&unaryTestNode->next, testExpression);
  return unaryTestNode;
}

static inline struct AbsynNode *UnaryOperator(struct AbsynNode *operator) {

  struct AbsynNode *unaryOperatorNode =
      mk_absyn_node(UNARY_OPERATOR_NODE, "UNARY_OPERATOR");
  add_absyn_node(&unaryOperatorNode->next, operator);
  return unaryOperatorNode;
}

static inline struct AbsynNode *TestOperator(struct AbsynNode *operator) {

  struct AbsynNode *testOperatorNode =
      mk_absyn_node(TEST_OPERATOR_NODE, "TEST_OPERATOR");
  add_absyn_node(&testOperatorNode->next, operator);
  return testOperatorNode;
}

static inline struct AbsynNode *
Redirection_InputRedirection(struct AbsynNode *inputRedirectionNode) {
  struct AbsynNode *redirectionNode =
      mk_absyn_node(REDIRECTION_NODE, "REDIRECTION");
  add_absyn_node(&redirectionNode->next, inputRedirectionNode);
  return redirectionNode;
}

static inline struct AbsynNode *
Redirection_OutputRedirection(struct AbsynNode *outputRedirectionNode) {
  struct AbsynNode *redirectionNode =
      mk_absyn_node(REDIRECTION_NODE, "REDIRECTION");
  add_absyn_node(&redirectionNode->next, outputRedirectionNode);
  return redirectionNode;
}

static inline struct AbsynNode *Redirection_AppendOutputRedirection(
    struct AbsynNode *appendOutputRedirectionNode) {
  struct AbsynNode *redirectionNode =
      mk_absyn_node(REDIRECTION_NODE, "REDIRECTION");
  add_absyn_node(&redirectionNode->next, appendOutputRedirectionNode);
  return redirectionNode;
}

static inline struct AbsynNode *
Redirection_HereDocument(struct AbsynNode *hereDocumentNode) {
  struct AbsynNode *redirectionNode =
      mk_absyn_node(REDIRECTION_NODE, "REDIRECTION");
  add_absyn_node(&redirectionNode->next, hereDocumentNode);
  return redirectionNode;
}

static inline struct AbsynNode *
Redirection_HereString(struct AbsynNode *hereStringNode) {
  struct AbsynNode *redirectionNode =
      mk_absyn_node(REDIRECTION_NODE, "REDIRECTION");
  add_absyn_node(&redirectionNode->next, hereStringNode);
  return redirectionNode;
}

static inline struct AbsynNode *InputRedirection(struct AbsynNode *fd,
                                                 struct AbsynNode *word) {

  struct AbsynNode *inputRedirectionNode =
      mk_absyn_node(INPUT_REDIRECTION_NODE, "INPUT_REDIRECTION");
  add_absyn_node(&inputRedirectionNode->next, fd);
  add_absyn_node(&inputRedirectionNode->next, word);
  return inputRedirectionNode;
}

static inline struct AbsynNode *OutputRedirection(struct AbsynNode *fd,
                                                  struct AbsynNode *word) {

  struct AbsynNode *outputRedirectionNode =
      mk_absyn_node(OUTPUT_REDIRECTION_NODE, "OUTPUT_REDIRECTION");
  add_absyn_node(&outputRedirectionNode->next, fd);
  add_absyn_node(&outputRedirectionNode->next, word);
  return outputRedirectionNode;
}

static inline struct AbsynNode *
AppendOutputRedirection(struct AbsynNode *fd, struct AbsynNode *word) {

  struct AbsynNode *appendOutputRedirectionNode = mk_absyn_node(
      APPEND_OUTPUT_REDIRECTION_NODE, "APPEND_OUTPUT_REDIRECTION");
  add_absyn_node(&appendOutputRedirectionNode->next, fd);
  add_absyn_node(&appendOutputRedirectionNode->next, word);
  return appendOutputRedirectionNode;
}

static inline struct AbsynNode *
DuplicateOutputRedirection(struct AbsynNode *fd, struct AbsynNode *word) {

  struct AbsynNode *duplicateOutputRedirectionNode = mk_absyn_node(
      DUPLICATE_OUTPUT_REDIRECTION_NODE, "DUPLICATE_OUTPUT_REDIRECTION");
  add_absyn_node(&duplicateOutputRedirectionNode->next, fd);
  add_absyn_node(&duplicateOutputRedirectionNode->next, word);
  return duplicateOutputRedirectionNode;
}

static inline struct AbsynNode *
DuplicateInputRedirection(struct AbsynNode *fd, struct AbsynNode *word) {

  struct AbsynNode *duplicateInputRedirectionNode = mk_absyn_node(
      DUPLICATE_INPUT_REDIRECTION_NODE, "DUPLICATE_INPUT_REDIRECTION");
  add_absyn_node(&duplicateInputRedirectionNode->next, fd);
  add_absyn_node(&duplicateInputRedirectionNode->next, word);
  return duplicateInputRedirectionNode;
}

static inline struct AbsynNode *HereDocument(struct AbsynNode *fd,
                                             struct AbsynNode *delimiter) {

  struct AbsynNode *hereDocumentNode =
      mk_absyn_node(HEREDOCUMENT_NODE, "HEREDOCUMENT");
  add_absyn_node(&hereDocumentNode->next, fd);
  add_absyn_node(&hereDocumentNode->next, delimiter);
  return hereDocumentNode;
}

static inline struct AbsynNode *HereString(struct AbsynNode *fd,
                                           struct AbsynNode *word) {

  struct AbsynNode *hereStringNode =
      mk_absyn_node(HERESTRING_NODE, "HERESTRING");
  add_absyn_node(&hereStringNode->next, fd);
  add_absyn_node(&hereStringNode->next, word);
  return hereStringNode;
}

static inline struct AbsynNode *Word_String(struct AbsynNode *stringNode) {
  struct AbsynNode *wordNode = mk_absyn_node(WORD_NODE, "WORD");
  add_absyn_node(&wordNode->next, stringNode);
  return wordNode;
}

static inline struct AbsynNode *
Word_AnyCharExceptSpace(struct AbsynNode *anyCharNode) {
  struct AbsynNode *wordNode = mk_absyn_node(WORD_NODE, "WORD");
  add_absyn_node(&wordNode->next, anyCharNode);
  return wordNode;
}

static inline struct AbsynNode *
Word_Substitution(struct AbsynNode *substitutionNode) {
  struct AbsynNode *wordNode = mk_absyn_node(WORD_NODE, "WORD");
  add_absyn_node(&wordNode->next, substitutionNode);
  return wordNode;
}

static inline struct AbsynNode *Eof() { return mk_absyn_node(EOF_NODE, "EOF"); }

#endif /* absyn.h */
