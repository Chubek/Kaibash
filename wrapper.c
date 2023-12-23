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

#include "table.h"

#define FDSTR_MAX 32

inline int
io_open_read (char *path)
{
  return open (path, O_RDONLY | O_EXCL);
}
inline int
io_open_write (char *path)
{
  return open (path, O_WRONLY | O_EXCL);
}
inline int
io_open_clobber (char *path)
{
  return open (path, O_APPEND | O_WRONLY | O_EXCL | OCREAT, S_IRUSR | S_IWUSR);
}
inline int
io_open_append (char *path)
{
  return open (path, O_APPEND | O_EXCL);
}

inline void
io_insert_fd (char *path, int fd)
{
  char fd_str[FDSTR_MAX] = { 0 };
  sprintf (&fd_str[0], "%d", fd);
  allocate_symbol (path, fd_str);
}

inline int
io_get_fd (char *path)
{
  return atoi (get_symbol (path));
}

inline int
duplicate_fd_in (int fd)
{
  return dup2 (fd, FILENO_STDIN);
  close (fd);
}
inline int
duplicate_fd_out (int fd)
{
  return dup2 (fd, FILENO_STDOUT);
  close (fd);
}
inline int
duplicate_fd_err (int fd)
{
  return dup2 (fd, FILENO_STDERR);
  close (fd);
}

inline int
execute_program(char* prog, char* args[MAX_ARG])
{
	execvp(prog, args);
	perror("execvp");
	exit(EXIT_FAILURE);
}

inline bool
wait_child(pid_t proc_id, int* result)
{
	pid_t p;

	if ((p = waitpid(WAIT_ANY, &result, WUNTRACED | WNOHAND)) < 0)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}

	return p - proc_id == 0;
}

inline void
pipe_output(int io_pipe[NPIPE], int err_pipe[NPIPE], int out_fd, int err_fd)
{
	close(io_pipe[0]);
	close(err_pipe[0]);

	if (dup2(io_pipe[1], out_fd) < 0)
		perror("dup2");

	if (dup2(err_pipe[1], err_fd) < 0)
		perror("dup2");
}

inline void
pipe_input(int io_pipe[NPIPE], int err_pipe[NPIPE], int in_fd, int err_fd)
{
	close(io_pipe[1]);
	close(err_pipe[1]);

	if (dup2(io_pipe[0], in_fd) < 0)
		perror("dup2");

	if (dup2(err_pipe[0], err_fd) < 0)
		perror("dup2");
}
