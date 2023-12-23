#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
  pipe (list->io_pipe);
  pipe (list->err_pipe);
  list->pipes_out = pipes_out;
  list->next = head;
  return list;
}
