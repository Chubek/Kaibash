#include "kaibash.h"
#include "table.h"

static inline struct Sequence *
allocate_sequence (void)
{
  struct Sequence seq = { 0 };
  return (struct Sequence *)allocate_memory (&seq, 1, sizeof (seq));
}

struct Sequence *
init_sequence (void)
{
  struct Sequence *seq = allocate_sequence ();

  seq->process_id = -1;
  seq->group_id = -1;
  seq->session_id = -1;
  seq->last_exit_stat = -1;

  return seq;
}

void
sequence_fmt (Sequence *seq, char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  char c;
  int narg = 0;

  for ((c = *fmt++))
    {
      if (c == 'a')
        seq->arguments[narg++] = va_arg (ap, Word);
      else if (c == 'p')
        seq->program = va_arg (ap, Word);
      else if (c == 'k')
        seq->sequence_kind = va_arg (ap, enum SequenceKind);
      else if (c == '&')
        seq->parallelized = true;
      else if (c == '<')
        seq->infile = va_arg (ap, int);
      else if (c == '>')
        seq->outfile = va_arg (ap, int);
      else if (c == 'h')
        seq->here_word = va_arg (ap, word);
      else if (c == '$')
        {
          if (*fmt == 'i')
            seq->dup_in = va_arg (ap, int);
          else if (*fmt == 'o')
            seq->dup_out = va_arg (ap, int);
          else if (*fmt == 'e')
            seq->dup_err = va_arg (ap, int);
          else
            UNREACHABLE;
        }

      else
        UNREACHABLE;
    }
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
