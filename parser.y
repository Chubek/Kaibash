%{
#include "tables.h"

enum NodeType
  {
    ATOM_SHELL,
    LINE_LIST,
    LINE_EMPTY,
    LIST_SINGLE,
    LIST_SEQUENCE,
    SEQ_SIMPLE,
    SEQ_PIPELINE,
    SEQ_PARALLEL,
    CMD_SIMPLE,
    CMD_COMPLEX,
    CMD_REDIR,
    CMD_PATTERN,
    CMD_SUBS,
    CMD_ASSIGN,
    ATOM_CMD,
    REDIR_INPUT,
    REDIR_OUTPUT,
    APPEND_OUTPUT,
    HERE_DOC,
    HERE_STR,
    FUNCTION,
    ATOM_PATT,
    SUB_SEQ,
    SUB_SHELL,
    SUB_PATT,
    SUB_PARAM,
    SUB_CONCAT,
    SUBS_ATOM,
    SUBPATT_QMARK_COLON,
    SUBPATT_PLUS_COLON,
    SUBPATT_DASH_COLON,
    SUBPATT_EQUAL_COLON,
    SUBPATT_SUFFIX_LARGE,
    SUBPATT_PREFIX_LARGE,
    SUBPATT_QUESTION_MARK,
    SUBPATT_PLUS_SIGN,
    SUBPATT_DASH_SIGN,
    SUBPATT_EQUAL_SIGN,
    SUBPATT_SUFFIX_SMALL,
    SUBPATT_PREFIX_SMALL,
    GLOB_KLEENE,
    GLOB_OPT,
    GLOB_CLOSURE,
    GLOB_SIMPLE,
    REGOPT_GROUP,
    REGOPT_RANGE,
    REGOPT_WORD,
    IF_CONDITIONAL,
    ELIF_CONDITIONAL,
    WHILE_LOOP,
    UNTIL_LOOP,
    FOR_LOOP,
    CASE,
    CASE_LIST,
    PATTERN_LIST,
    PATTERN_ATOM,
    LOOP,
    ATOM_WORD_LIT,
    ATOM_NUM_LIT,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_MOD,
    EXPR_SHR,
    EXPR_SHL,
    EXPR_POW,
    ATOM_PATT_LIT,
  }

struct Atom
  {
    enum NodeType type;
    char *value;
  struct Atom *child;
  struct Atom *sibling;
  }

struct Atom *make_atom (enum NodeType type, char *value)
  {
  struct Atom *atom
        = (struct Atom *)allocate_memory_zero (sizeof (struct Atom));
    atom->type = type;
    atom->value = (value != NULL) ? STRUDUP (value) : NULL;
    atom->child = NULL;
    atom->sibling = NULL;
    return atom;
  }

struct Atom *make_line (enum NodeType type, struct Atom * command_list)
  {
    return make_atom (type, NULL, command_list, NULL);
  }

  void add_word_list (struct Atom * *list, char *word)
  {
  struct Atom *word_atom = make_atom (ATOM_WORD_LIT, word);

    if (*list == NULL)
      {
        *list = word_atom;
      }
    else
      {
      struct Atom *current = *list;
        while (current->sibling != NULL)
          {
            current = current->sibling;
          }
        current->sibling = word_atom;
      }
  }

  void add_pattern_list (struct Atom * *list, struct Atom * pattern)
  {
    if (*list == NULL)
      {
        *list = pattern;
      }
    else
      {
      struct Atom *current = *list;
        while (current->sibling != NULL)
          {
            current = current->sibling;
          }
        current->sibling = pattern;
      }
  }

struct Atom *make_regopt (enum NodeType type, char *word, char *range)
  {
  struct Atom *regopt = make_atom (type, word);
    if (range != NULL)
      {
        regopt->child = make_atom (REGOPT_RANGE, range);
      }
    return regopt;
  }

struct Atom *make_seq (enum NodeType type, struct Atom * left,
                       struct Atom * right)
  {
  struct Atom *seq = make_atom (type, NULL);
    seq->child = left;
    seq->sibling = right;
    return seq;
  }

struct Atom *add_shell_line (struct Atom * *shell, struct Atom * line)
  {
    if (*shell == NULL)
      {
        *shell = line;
      }
    else
      {
      struct Atom *current = *shell;
        while (current->sibling != NULL)
          {
            current = current->sibling;
          }
        current->sibling = line;
      }
    return *shell;
  }

struct Atom *make_list (enum NodeType type, struct Atom * left,
                        struct Atom * right)
  {
  struct Atom *list = make_atom (type, NULL);
    list->child = left;
    list->sibling = right;
    return list;
  }

struct Atom *add_shell_line (struct Atom * *shell, struct Atom * line)
  {
    if (*shell == NULL)
      {
        *shell = line;
      }
    else
      {
      struct Atom *current = *shell;
        while (current->sibling != NULL)
          {
            current = current->sibling;
          }
        current->sibling = line;
      }
    return *shell;
  }

struct Atom *make_redirect (enum NodeType type, struct Atom * num,
                              char *word)
  {
  struct Atom *redirect = make_atom (type, word);
    redirect->child = num;
    return redirect;
  }

struct Atom *add_command_pipeline (struct Atom * *pipeline,
                                   struct Atom * command)
  {
    if (*pipeline == NULL)
      {
        *pipeline = command;
      }
    else
      {
      struct Atom *current = *pipeline;
        while (current->sibling != NULL)
          {
            current = current->sibling;
          }
        current->sibling = command;
      }
    return *pipeline;
  }

struct Atom *make_cmd (enum NodeType type, struct Atom * child, char *value)
  {
  struct Atom *cmd = make_atom (type, value);
    cmd->child = child;
    return cmd;
  }

struct Atom *make_here_str (struct Atom * command, struct Atom * here_str)
  {
  struct Atom *here_str_atom = make_atom (HERE_STR, NULL);
    here_str_atom->child = command;
    here_str_atom->sibling = here_str;
    return here_str_atom;
  }

struct Atom *make_sub_atom (enum NodeType type, struct Atom * child,
                              char *value)
  {
  struct Atom *sub_atom = make_atom (type, value);
    sub_atom->child = child;
    return sub_atom;
  }

struct Atom *make_subpatt (enum NodeType type, struct Atom * child)
  {
  struct Atom *subpatt = make_atom (type, NULL);
    subpatt->child = child;
    return subpatt;
  }

struct Atom *make_glob (enum NodeType type, struct Atom * child)
  {
  struct Atom *glob = make_atom (type, NULL);
    glob->child = child;
    return glob;
  }

struct Atom *make_case (struct Atom * pattern_list, char *value)
  {
  struct Atom *case_atom = make_atom (CASE, value);
    case_atom->child = pattern_list;
    return case_atom;
  }

struct Atom *make_while_loop (struct Atom * condition,
                              struct Atom * command_list)
  {
  struct Atom *while_loop = make_atom (WHILE_LOOP, NULL);
    while_loop->child = condition;
    while_loop->sibling = command_list;
    return while_loop;
  }

struct Atom *make_until_loop (struct Atom * condition,
                              struct Atom * command_list)
  {
  struct Atom *until_loop = make_atom (UNTIL_LOOP, NULL);
    until_loop->child = condition;
    until_loop->sibling = command_list;
    return until_loop;
  }

struct Atom *make_function (char *name, struct Atom *line)
  {
  struct Atom *function = make_atom (FUNCTION, name);
    function->child = line;
    return function;
  }

struct Atom *make_conditional (struct Atom * condition,
                               struct Atom * true_branch,
                               struct Atom * false_branch)
  {
  struct Atom *conditional = make_atom (IF_CONDITIONAL, NULL);
    conditional->child = condition;
    conditional->sibling
        = make_atom (LINE_LIST, NULL); // Placeholder for true_branch, if any
    if (true_branch != NULL)
      {
        conditional->sibling->child = true_branch;
        if (false_branch != NULL)
          {
            conditional->sibling->sibling = false_branch;
          }
      }
    return conditional;
  }

struct Atom *add_expression (struct Atom * *root, char *value,
                               enum NodeType type)
  {
  struct Atom *new_expr = make_atom (type, value);
    if (*root == NULL)
      {
        *root = new_expr;
      }
    else
      {
      struct Atom *current = *root;
        while (current->sibling != NULL)
          {
            current = current->sibling;
          }
        current->sibling = new_expr;
      }
    return new_expr;
  }

struct Atom *make_for_loop (char *variable, struct Atom *word_list,
                            struct Atom *command_list)
  {
  struct Atom *for_loop = make_atom (FOR_LOOP, variable);
    for_loop->child = word_list;
    for_loop->sibling = command_list;
    return for_loop;
  }

struct Atom *add_elif_list (struct Atom * *head, struct Atom * condition,
                            struct Atom * true_branch)
  {
  struct Atom *new_elif = make_atom (ELIF_CONDITIONAL, NULL);
    new_elif->child = condition;
    new_elif->sibling
        = make_atom (LINE_LIST, NULL); // Placeholder for true_branch, if any
    if (*head == NULL)
      {
        *head = new_elif;
      }
    else
      {
      struct Atom *current = *head;
        while (current->sibling != NULL)
          {
            current = current->sibling;
          }
        current->sibling = new_elif;
      }
    if (true_branch != NULL)
      {
        new_elif->sibling->child = true_branch;
      }
    return *head;
  }

struct Atom *make_elif (struct Atom * condition, struct Atom * true_branch)
  {
  struct Atom *elif = make_atom (ELIF_CONDITIONAL, NULL);
    elif->child = condition;
    elif->sibling = true_branch;
    return elif;
  }

struct Atom *make_case (struct Atom * pattern_list, char *case_word)
  {
  struct Atom *case_atom = make_atom (CASE, case_word);
    case_atom->child = pattern_list;
    return case_atom;
  }

struct Atom *make_here_doc (struct Atom * command, struct Atom * here_word,
                            struct Atom * word_list, char *word)
  {
  struct Atom *here_doc = make_atom (HERE_DOC, word);
    here_doc->child = command;
    here_doc->sibling = here_word;
    if (word_list != NULL)
      {
      struct Atom *last_sibling = here_doc;
        while (last_sibling->sibling != NULL)
          {
            last_sibling = last_sibling->sibling;
          }
        last_sibling->sibling = word_list;
      }
    return here_doc;
  }

struct Atom *make_seq (enum NodeType type, struct Atom * child,
                       struct Atom * sibling)
  {
  struct Atom *seq = make_atom (type, NULL);
    seq->child = child;
    seq->sibling = sibling;
    return seq;
  }

%}

%token SEMICOLON 
%token PIPE 
%token AMPERSAND 
%token IF 
%token THEN
%token ELSE 
%token FI 
%token WHILE 
%token UNTIL 
%token DO 
%token DONE
%token FOR 
%token IN 
%token CASE 
%token ESAC 
%token OPEN_CLOSE
%token ATSIGN_LCURLY 
%token DOLLAR_LCURLY 
%token DOLLAR_LPAREN
%token DOUBLE_LPAREN 
%token DOUBLE_RPAREN 
%token REDIRECT_OUTPUT
%token REDIRECT_IO_DOC 
%token REDIRECT_IO_STR 
%token NAME 
%token NUM
%token SINGLE 
%token WORD 
%token PATTERN 
%token IDENT
%token SHL 
%token SHR
%token POW

%union
{
struct Word word;
struct Atom atom;
}

%type<atom> command pipeline conditional loop case_atom literal substitution_atom pattern_atom glob_pattern regopt
%type<word> WORD 
%type<word> NUM 
%type<word> PATTERN

%%

program:
    /* empty */ { $$ = make_atom(ATOM_SHELL, NULL); }
    | program line { $$ = add_shell_line(&$1, $2); }

line:
    command_list '\n' { $$ = make_line(LINE_LIST, $1); }
    | '\n' { $$ = make_line(LINE_EMPTY, NULL); }

command_list:
    command { $$ = make_list(LIST_SINGLE, $1, NULL); }
    | command_list ';' command { $$ = make_list(LIST_SEQUENCE, $1, $3); }

command:
    simple_command { $$ = make_seq(SEQ_SIMPLE, $1, NULL); }
    | command '|' command { $$ = make_seq(SEQ_PIPELINE, $1, $3); }
    | command '&' command { $$ = make_seq(SEQ_PARALLEL, $1, $3); }

simple_command:
    WORD { $$ = make_cmd(CMD_SIMPLE, $1, NULL); }
    | simple_command WORD { $$ = make_cmd(CMD_COMPLEX, $1, $2); }
    | simple_command redirection { $$ = make_cmd(CMD_REDIR, $1, $2); }
    | simple_command pattern_atom { $$ = make_cmd(CMD_PATTERN, $1, $2); }
    | simple_command substitution_atom { $$ = make_cmd(CMD_SUBS, $1, $2); }
    | IDENT simple_command { $$ = make_cmd(CMD_ASSIGN, $2, $1); }

pipeline:
    simple_command { $$ = make_atom(ATOM_CMD, $1); }
    | pipeline '|' simple_command { $$ = add_command_pipeline(&$1, $3); }

redirection:
    '<' WORD { $$ = make_redirect(REDIR_INPUT, NULL, $2); }
    | '>' WORD { $$ = make_redirect(REDIR_OUTPUT, NULL, $2); }
    | REDIRECT_OUTPUT WORD { $$ = make_redirect(APPEND_OUTPUT, NULL, $2); }
    | NUM '<' WORD { $$ = make_redirect(REDIR_INPUT, $1, $3); }
    | NUM '>' WORD { $$ = make_redirect(REDIR_OUTPUT, $1, $3); }
    | NUM REDIRECT_OUTPUT WORD { $$ = make_redirect(APPEND_OUTPUT, $1, $3); }

io_here:
    command here_word '\n' word_list WORD { $$ = make_here_doc($1, $2, $4, $5); }
    | command here_word { $$ = make_here_str($1, $2); }

here_word:
    REDIRECT_IO_STR WORD { $$ = make_atom(HERE_STR_WORD, $2); }
    | REDIRECT_IO_DOC WORD { $$ = make_atom(HERE_DOC_WORD, $2); }

function:
    NAME OPEN_CLOSE '{' line '}' { $$ = make_function($1, $4); }

pattern_atom:
    PATTERN { $$ = make_atom(ATOM_PATT, $1); }

substitution_atom:
    DOLLAR_LCURLY command_list '}' { $$ = make_sub_atom(SUB_SEQ, $1, NULL); }
    | DOLLAR_LPAREN command_list ')' | DOLLAR_LCURLY param substitution_pattern '}'
    {
        $$ = make_sub_atom(SUB_SHELL, $2, NULL);
    }
    | '$' DOUBLE_LPAREN expression DOUBLE_RPAREN
    {
        $$ = make_sub_atom(SUB_PATT, $2, $3);
    }
    | '$' NAME { $$ = make_sub_atom(SUB_PARAM, $2, NULL); }
    | substitution_atom pattern_atom { $$ = make_subs_atom(SUB_CONCAT, $1, $2); }

substitution_pattern:
    ":?" glob_pattern { $$ = make_subpatt(SUBPATT_QMARK_COLON, $2); }
    | ":+" glob_pattern { $$ = make_subpatt(SUBPATT_PLUS_COLON, $2); }
    | ":-" glob_pattern { $$ = make_subpatt(SUBPATT_DASH_COLON, $2); }
    | ":=" glob_pattern { $$ = make_subpatt(SUBPATT_EQUAL_COLON, $2); }
    | "##" glob_pattern { $$ = make_subpatt(SUBPATT_SUFFIX_LARGE, $2); }
    | "%%" glob_pattern { $$ = make_subpatt(SUBPATT_PREFIX_LARGE, $2); }
    | '?' glob_pattern { $$ = make_subpatt(SUBPATT_QUESTION_MARK, $2); }
    | '+' glob_pattern { $$ = make_subpatt(SUBPATT_PLUS_SIGN, $2); }
    | '-' glob_pattern { $$ = make_subpatt(SUBPATT_DASH_SIGN, $2); }
    | '=' glob_pattern { $$ = make_subpatt(SUBPATT_EQUAL_SIGN, $2); }
    | '#' glob_pattern { $$ = make_subpatt(SUBPATT_SUFFIX_SMALL, $2); }
    | '%' glob_pattern { $$ = make_subpatt(SUBPATT_PREFIX_SMALL, $2); }

glob_pattern:
    regopt '*' { $$ = make_glob(GLOB_KLEENE, $1); }
    | regopt '?' { $$ = make_glob(GLOB_OPT, $1); }
    | regopt '+' { $$ = make_glob(GLOB_CLOSURE, $1); }
    | regopt { $$ = make_glob(GLOB_SIMPLE, $1); }

regopt:
    '[' WORD ']' { $$ = make_regopt(REGOPT_GROUP, $2, NULL); }
    | '[' PATTERN '-' PATTERN ']' { $$ = make_regopt(REGOPT_RANGE, $2, $4); }
    | WORD { $$ = make_regopt(REGOPT_WORD, $1, NULL); }

conditional:
    IF command_list THEN command_list ELSE command_list FI
    {
        $$ = make_conditional($2, $4 $6);
    }
    | IF command_list THEN command_list FI
    {
        $$ = make_conditional($2, $4, NULL);
    }

elif:
    /* empty */
    | ELIF command_list THEN command_list
    {
        $$ = make_elif($2, $4);
    }
    | ELIF command_list THEN command_list elif
    {
        $$ = add_elif_list(&$5, $2, $4);
    }

loop:
    WHILE command_list DO command_list DONE
    {
        $$ = make_while_loop($2, $4);
    }
    | UNTIL command_list DO command_list DONE { $$ = make_until_loop($2, $4); }
    | FOR WORD IN word_list DO command_list DONE
    {
        $$ = make_for_loop($2, $4, $6);
    }

case_atom:
    CASE WORD IN case_list ESAC { $$ = make_case($4, $2); }

case_list:
    pattern_list ')' command_list case_list
    {
        $$ = add_case_list(&$0, $3, $1, $2);
    }
    | /* empty */;

pattern_list:
    pattern_list '|' pattern { $$ = add_pattern_list(&$1, $3); }
    | pattern { $$ = make_atom(ATOM_PATT, $1); }

pattern:
    PATTERN { $$ = make_atom(ATOM_PATT_LIT, $1); }
    | PATTERN '|' pattern { $$ = add_pattern(&$3, $1); }

word_list:
    word_list WORD { $$ = add_word_list(&$1, $2); }
    | WORD { $$ = make_atom(ATOM_WORD_LIT, $1); }

expression:
    NUM '+' expression { $$ = add_expression(&$3, $1, EXPR_ADD); }
    | NUM '-' expression { $$ = add_expression(&$3, $1, EXPR_SUB); }
    | NUM '*' expression { $$ = add_expression(&$3, $1, EXPR_MUL); }
    | NUM '/' expression { $$ = add_expression(&$3, $1, EXPR_DIV); }
    | NUM '%' expression { $$ = add_expression(&$3, $1, EXPR_MOD); }
    | NUM SHR expression { $$ = add_expression(&$3, $1, EXPR_SHR); }
    | NUM SHL expression { $$ = add_expression(&$3, $1, EXPR_SHL); }
    | NUM POW expression { $$ = add_expression(&$3, $1, EXPR_POW); }
    | NUM { $$ = make_atom(ATOM_NUM_LIT, $1); }


%%

#define NPIPE 2

struct Command
{
  char *prefix;
  char *arguments[ARG_MAX];
  int infd, outfd, errfd;
  bool parallel;
  struct Command *next;
};

struct List
{
  struct Command *prelude;
  struct Command *flow;
  struct Command *epilogue;
  int io_pipe[NPIPE];
  int err_pipe[NPIPE];
  bool pipes_out;
  struct List *next;
};

struct ShellExec
{
  char working_directory[FILENAME_MAX + 1];
  char term_fname[L_ctermid + 1];
  int last_exit_stat;
  int term_fdesc;
  int is_interactive;
  pid_t group_id;
  pid_t foreground_group_id;
  pid_t foreground_session_id;
  struct Symtab *symtab;
  struct List *list;
};

struct ShellExec *
initialize_shell ()
{
  struct ShellExec *shellEnv
      = (struct ShellExec *)allocate_memory_zero (
          sizeof (struct ShellExec));
  return shellEnv;
}

struct Command *
add_command (struct Command *head, char *prefix, char *arguments[], int infd,
             int outfd, int errfd, bool parallel)
{
  struct Command *cmd
      = (struct Command *)allocate_memory_zero (sizeof (struct Command));
  cmd->prefix = prefix;

  for (int i = 0; arguments[i] != NULL; ++i)
    {
      cmd->arguments[i] = STRDUP (arguments[i]);
    }
  cmd->infd = infd;
  cmd->outfd = outfd;
  cmd->errfd = errfd;
  cmd->parallel = parallel;
  cmd->next = head;
  return cmd;
}

struct List *
add_list (struct List *head, struct Command *prelude, struct Command *flow,
          struct Command *epilogue, bool pipes_out)
{
  struct List *list
      = (struct List *)allocate_memory_zero (sizeof (struct List));
  list->prelude = prelude;
  list->flow = flow;
  list->epilogue = epilogue;
  list->pipes_out = pipes_out;
  list->next = head;
  return list;
}

void
walk_tree (struct Atom *node, int depth)
{
  if (node == NULL)
    {
      return;
    }

  walk_tree (node->child, depth + 1);
  walk_tree (node->sibling, depth);
}
