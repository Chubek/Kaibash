#include "kaibash.h"
#include "tables.h"

struct CommandRedirection *
create_command_redirection(enum RedirectionType type, const char *source,
                           const char *destination, const char *here_doc_marker,
                           struct Atom *here_doc_content) {
  struct CommandRedirection *redirection =
      (struct CommandRedirection *)allocate_memory_zero(
          sizeof(struct CommandRedirection));
  redirection->redirection_type = type;
  redirection->source = strdup(source);
  redirection->destination = strdup(destination);
  redirection->here_doc_marker = strdup(here_doc_marker);
  redirection->here_doc_content = here_doc_content;
  return redirection;
}

struct Atom *create_atom(enum AtomKind kind) {
  struct Atom *atom = (struct Atom *)allocate_memory_zero(sizeof(struct Atom));
  atom->next_atom = NULL;
  atom->atom_kind = kind;
  return atom;
}

struct Atom *create_command_atom(const char *command, char *arguments[ARG_MAX],
                               struct CommandRedirection *redirections) {
  struct Atom *atom = create_atom(COMMAND);
  struct CommandAtom *commandAtom = &(atom->atom_node.command_atom);
  commandAtom->command = strdup(command);
  for (int i = 0; i < ARG_MAX; ++i) {
    if (arguments[i] != NULL) {
      commandAtom->arguments[i] = strdup(arguments[i]);
    } else {
      commandAtom->arguments[i] = NULL;
    }
  }
  commandAtom->redirections = redirections;
  return atom;
}

struct Atom *create_pipeline_atom(struct Atom *left, struct Atom *right) {
  struct Atom *atom = create_atom(PIPELINE);
  struct PipelineAtom *pipelineAtom = &(atom->atom_node.pipeline_atom);
  pipelineAtom->left = left;
  pipelineAtom->right = right;
  return atom;
}

struct Atom *create_conditional_atom(struct Atom *condition,
                                     struct Atom *true_branch,
                                     struct Atom *false_branch) {
  struct Atom *atom = create_atom(CONDITIONAL);
  struct ConditionalAtom *conditionalAtom = &(atom->atom_node.conditional_atom);
  conditionalAtom->condition = condition;
  conditionalAtom->true_branch = true_branch;
  conditionalAtom->false_branch = false_branch;
  return atom;
}

struct Atom *create_case_atom(Word word, struct Atom *list,
                              struct Atom *cases) {
  struct Atom *atom = create_atom(CASE);
  struct CaseAtom *caseAtom = &(atom->atom_node.case_atom);
  caseAtom->word = strdup(word);
  caseAtom->list = list;
  caseAtom->cases = cases;
  return atom;
}

struct Atom *createLoopAtom(struct Atom *condition, struct Atom *body,
                            bool unless) {
  struct Atom *atom = create_atom(LOOP);
  struct LoopAtom *loopAtom = &(atom->atom_node.loop_atom);
  loopAtom->condition = condition;
  loopAtom->body = body;
  loopAtom->unless = unless;
  return atom;
}

struct Atom *create_literal_atom(enum LiteralKind kind, const char *value) {
  struct Atom *atom = create_atom(LITERAL);
  struct LiteralAtom *literalAtom = &(atom->atom_node.literal_atom);
  literalAtom->literal_kind = kind;

  if (kind == STRING) {
    literalAtom->literal_node.string = strdup(value);
  } else if (kind == INTEGER) {
    literalAtom->literal_node.integer = atoi(value);
  }

  return atom;
}

struct Atom *create_substitution_atom(enum SubstitionKind kind,
                                      struct Atom *substitution_node) {
  struct Atom *atom = create_atom(SUBSTITUTION);
  struct SubstitutionAtom *substitutionAtom =
      &(atom->atom_node.substitution_atom);
  substitutionAtom->substitution_kind = kind;
  substitutionAtom->substitution_node = substitution_node;
  return atom;
}

struct Atom *create_pattern_atom(struct Pattern *pattern_node) {
  struct Atom *atom = create_atom(PATTERN);
  atom->atom_node.pattern_atom = pattern_node;
  return atom;
}

struct Pattern *create_pattern(enum PatternSigil sigil, int kind,
                               const char *value, size_t length) {
  struct Pattern *pattern =
      (struct Pattern *)allocate_memory_zero(sizeof(struct Pattern));
  pattern->sigil = sigil;
  pattern->kind = kind;
  pattern->value = strdup(value);
  pattern->length = length;
  return pattern;
}
