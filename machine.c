


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
