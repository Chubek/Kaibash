#include "kaibash.h"
#include "tables.h"

static inline void
rm_literal_prefix (char *word, char *literal)
{
  for (char c1 = *word++, c2 = *literal++; c1 == c2 && c2;
       c1 = *word++, c2 = *literal++)
    ;
}

static inline void
rm_literal_suffix (char *word, char *literal, size_t lword, size_t llit)
{
  for (char c1 = *(word + --lword), c2 = *(literal + --llit);
       c1 == c2 && c2 != *literal;
       c1 = *(word + --lword), c2 = *(literal + --llit))
    ;
  word = &word[lword];
}

static inline void
rm_epsilon_prefix (char *word, char *epsilon)
{
  for (char c1 = *word; strchr (epsilon, c1) != NULL;)
    {
      for (char c2 = *epsilon++, *p = epsilon; c2; c2 = *epsilon++)
        !(c1 ^ c2) ? *word++ : (0);
      epsilon = p;
    }
}

static inline void
rm_epsilon_suffix (char *word, char *epsilon, size_t lword)
{
  for (char c1 = *(word + --lword); strchr (epsilon, c1) != NULL;)
    {
      for (char c2 = *epsilon++, *p = epsilon; c2; c2 = *epsilon++)
        !(c1 ^ c2) ? --lword : (0);
      epsilon = p;
    }
  word = &word[lword];
}

static inline void
rm_group_prefix (char *word, char *group)
{
  for (char c = *group++, *p; c; c = *group++)
    if ((p = strchr (word, c)) != NULL)
      word = p;
}

static inline void
rm_group_suffix (char *word, char *group)
{
  for (char *c = *group++, *p; c; c = *group++)
    if ((p = strrchr (word, c)) != NULL)
      word = &word[(size_t)(p - word)];
}

struct Pipeline *
init_pipeline (struct Sequence *seq, bool banged)
{
  struct Pipeline *pln = allocate_memory_zero (sizeof (struct Pipeline));

  pln->sequence = seq;
  pln->banged = banged;

  return pln;
}

void
run_pipeline (struct Pipeline *pln)
{
  if (pln->next == NULL)
    {
      pln->sequence->infile = pln->pipe_in_fd;
      pln->sequence->outfile = pln->pipe_out_fd;
      pln->sequence->errfile = pln->pipe_err_fd;

      exec_sequence (pln->sequence, pln->banged ? ~(&pln->last_exit_stat)
                                                : &pln->last_exit_stat);
      return;
    }

  if ((pipe (pln->pipe_io)) < 0)
    perror ("pipe");

  if (pln->pipe_err)
    if ((pipe (pln->pipe_err)) < 0)
      perror ("pipe");

  pid_t child_pid;

  if ((child_pid = fork ()) < 0)
    perror ("fork");

  if (!child_pid)
    {
      close (pid->pipe_io[1]);
      dup2 (pln->pipe_io[0], pln->next->pipe_in_fd);
      close (pln->pipe_io[0]);

      if (pln->pipe_err)
        {
          close (pid->pipe_err[1]);
          dup2 (pln->pipe_err[0], pln->pipe_fd_err);
          close (pln->pipe_err[0]);
        }

      pln->sequence->infile = pln->next->pipe_in_fd;
      pln->sequence->outfile = pln->next->pipe_out_fd;
      pln->sequence->errfile = pln->next->pipe_err_fd;

      exec_sequence (pln->next->sequence, pln->next->banged
                                              ? ~(&pln->next->last_exit_stat)
                                              : &pln->next->last_exit_stat);
    }

  close (pid->pipe_io[0]);
  dup2 (pln->pipe_io[1], pln->pipe_out_fd);
  close (pln->pipe_io[1]);

  if (pln->pipe_err)
    {
      close (pid->pipe_err[0]);
      dup2 (pln->pipe_err[1], pln->pipe_fd_err);
      close (pln->pipe_err[1]);
    }

  pln->sequence->infile = pln->pipe_in_fd;
  pln->sequence->outfile = pln->pipe_out_fd;
  pln->sequence->errfile = pln->pipe_err_fd;

  exec_sequence (pln->sequence, pln->next->banged ? ~(&pln->last_exit_stat)
                                                  : &pln->last_exit_stat);
}

struct Sequence *
init_sequence (void)
{
  struct Sequence *seq = allocate_memory_zero (sizeof (struct Sequence));

  seq->process_id = -1;
  seq->group_id = -1;
  seq->session_id = -1;
  seq->last_exit_stat = -1;

  return seq;
}

void
sequence_exec_simple (struct Sequence *seq)
{
  execvp (seq->program, seq->arguments);
  perror ("execvp");
  exit (EXIT_FAILURE);
}

void
sequence_exec_dup (struct Sequence *seq)
{
  if (seq->infile != FILENO_STDIN)
    {
      dup2 (seq->infile, FILENO_STDIN);
      close (seq->infile);
    }

  if (seq->outfile != FILENO_STDOUT)
    {
      dup2 (seq->outfile, FILENO_STDOUT);
      close (seq->outfile);
    }

  if (seq->errfile != FILENO_STDERR)
    {
      dup2 (seq->errfile, FILENO_STDERR);
      close (seq->errfile);
    }

  execvp (seq->program, seq->arguments);
  perror ("execvp");
  exit (EXIT_FAILURE);
}

void
sequence_exec_parallel (struct Sequence *seq, int *exit_stat)
{
  if (seq->infile != FILENO_STDIN)
    {
      dup2 (seq->infile, FILENO_STDIN);
      close (seq->infile);
    }

  if (seq->outfile != FILENO_STDOUT)
    {
      dup2 (seq->outfile, FILENO_STDOUT);
      close (seq->outfile);
    }

  if (seq->errfile != FILENO_STDERR)
    {
      dup2 (seq->errfile, FILENO_STDERR);
      close (seq->errfile);
    }

  pid_t child_pid;

  if ((child_pid = fork ()) < 0)
    perror ("fork");

  if (!child_pid)
    {
      execvp (seq->program, seq->arguments);
      perror ("execvp");
      exit (EXIT_FAILURE);
    }

  if (waitpid (child_pid, exit_stat, 0) < 0)
    perror ("waitpid");
}

void
sequence_exec_here (struct Sequence *seq, int *exit_stat)
{
  int io_pipe[2];
  pid_t child_pid;

  if (pipe (io_pipe) < 0)
    perror ("pipe");

  if ((child_pid = fork ()) < 0)
    perror ("fork");

  if (!child_pid)
    {
      close (io_pipe[1]);
      dup2 (io_pipe[0], STDIN_FILENO);
      close (io_pipe[0]);

      execvp (seq->program, seq->arguments);
      perror ("exevp");
      exit (EXIT_FAILURE);
    }

  close (io_pipe[0]);
  FILE *wstream = fdopen (io_pipe[1], "w");
  fputs (seq->io_word, wstream);
  fclose (wstream);
  close (io_pipe[0]);

  if (waitpid (child_pid, exit_stat, 0) < 0)
    perror ("waitpid");
}

void
sequence_read_io_word (struct Sequence *seq)
{
  FILE *rstream = fopen (io->word, "r");
  ling rlen;

  if (fseek (rstream, 0, SEEK_END) < 0)
    perror ("fseek");

  if ((rlen = ftell (rstream)) < 0)
    perror ("ftell");

  rewind (rstream);

  char *conents = calloc (1, rlen);
  fgets (&contents[0], rlen, rstream);

  seq->io_word = STRNDUP (&contents[0], rlen);

  free (contents);
  fclose (rstream);
}

struct Shell *
init_shell (bool login)
{
  struct Shell *sh = allocate_memory_zero (sizeof (struct Shell *));

  sh->process_id = getpid ();
  sh->group_id = getgpid ();
  sh->session_id = getsid ();
  sh->interactive = isatty (STDIN_FILENO);
  sh->login = login;
}

void
init_pipelines (struct Shell *sh, struct Pipeline *pln_root)
{
  sh->pipelines = pln_root;
}

void
init_termfd (struct Shell *sh)
{
  static char cterm_name[L_ctermid + 1] = { 0 };
  char *cterm_fname = ctermid (&cterm_name[0]);
  sh->termfd = open (&cterm_fname[0], O_RDWR | O_EXCL);
}

void
init_signals (void)
{
  signal (SIGINT, SIG_IGN);
  signal (SIGQUIT, SIG_IGN);
  signal (SIGTSTP, SIG_IGN);
  signal (SIGTTIN, SIG_IGN);
  signal (SIGTTOU, SIG_IGN);
  signal (SIGCHLD, SIG_IGN);
}

void
init_atexit (void)
{
  atexit (dump_memory);
}

void
init_terminal (struct Shell *sh)
{
  tcsetpgrp (sh->termfd, sh->group_id);
  tcgetattr (sh->termfd, &sh->termio);
}
~~
