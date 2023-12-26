#include "tree.h"
#include "tables.h"

struct AbsynNode *create_node(enum NodeType type) {
  struct AbsynNode *node =
      (struct AbsynNode *)allocate_memory(sizeof(struct AbsynNode));
  if (node == NULL) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  node->type = type;
  node->left = NULL;
  node->right = NULL;
  node->value = NULL;
  return node;
}

struct AbsynNode *create_if_statement_node(struct AbsynNode *condition,
                                           struct AbsynNode *if_block,
                                           struct AbsynNode *else_block) {
  struct AbsynNode *node = create_node(IF_STATEMENT);
  node->left = condition;
  node->right = create_node(COMPOUND_COMMAND);
  node->right->left = if_block;
  node->right->right = else_block;
  return node;
}
