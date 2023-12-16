#include "tables.h"
#include "kaibash.h"

int
simple_few_loop(char* prog, char** argv)
{
	pid_t child_pid;
	int   exit_stat;

	if ((child_pid = fork()) < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (!child_pid)
	{
		if (execvp(prog, argv) < 0)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}

	return (waitpid(child_pid, &exit_stat, 0) == child_pid)
		? exit_stat
		: EXIT_FAILURE;
}

int
io_few_loop(char* prog, 
		char** argv, 
		int fd,
		int fd_copy,
		bool no_clobber,
		char* filename,
		char* word,
		size_t lenword,
		bool do_pipe,
		enum IoMode mode)

{
	int fcntl_flags 		= 0;
	int dest_fdesc  		= 0;
	int child_pid 			= 0;
	int exit_stat 			= 0;
	mode_t fcntl_modes 		= 0;

	int io_pipe[2] = {0};
	int err_pipe[2] = {0};

	if (do_pipe)
	{
		if (pipe(io_pipe) < 0)
			perror("pipe");
		if (pipe(err_pipe) < 0)
			perror("pipe");
	}

	switch (mode)
	{
		case INTO:
		case DUP:
			fcntl_flags = no_clobber ? 0 : O_WRONLY | O_CREAT | O_EXCL;
			fcntl_modes = no_clobber ? 0 : S_IWUSR | S_IRUSR;
			break;
		case UNTO:
			fcntl_flags = O_RDONLY | O_EXCL;
			break;
		case CLOBBERER:
			fcntl_flags = O_WRONLY | O_CREAT | O_EXCL;
			fcntl_modes = S_IWUSR | S_IRUSR;
			break;
		case APPEND:
			fcntl_flags = O_APPEND | O_EXCL;
			break;
		case RW:
			fcntl_flags = O_RDWR | O_EXCL;
			break;
		case HERE_STR:
		case HERE_DOC:
			fcntl_flags = O_RDWR | O_CREAT | O_EXCL;
		default:
			break;
	}

	if (filename == NULL)
	{
		FILE* tfile = tmpfile();
		if (word != NULL)
			fwrite(word, lenword, 1, tfile);
		dest_fdesc = fileno(tfile);
		goto dup_fd;
	}

	if (fcntl_modes == 0 && no_clobber)
	{
		fprintf(stderr, "No clobber is set, operation is disallowed\n");
		exit(EXIT_FAILURE);
	}

	dest_fdesc = fcntl_modes 
			? open(filename, fcntl_flags, fcntl_modes)
			: open(filename, fcntl_flags);
dup_fd:
	if (dest_fdesc < 0)
		perror("open");
	
	if (fd > 0)
		dup2(dest_fdesc, fd) < 0 ? perror("dup2") : (0);
	if (fd_copy > 0)
		dup2(fd, fd_copy) < 0 ? perror("dup2") : (0);

	if ((child_pid = fork()) < 0)
		perror("fork");

	else if (!child_pid)
	{
		if (do_pipe)
		{
			close(io_pipe[0]);
			close(err_pipe[1]);

			dup2(io_pipe[1], STDOUT_FILENO) < 0
				? perror("dup2")
				: (0);
			dup2(err_pipe[0], STDERR_FILENO) < 0
				? perror("dup2")
				: (0);
		}

		if (execvp(prog, argv) < 0)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}

	}

	if (do_pipe)
	{
		close(io_pipe[1]);
		close(err_pipe[0]);

		dup2(io_pipe[0], STDIN_FILENO);
		dup2(err_pipe[1], STDERR_FILENO);
	}

	close(dest_fdesc);

	return (waitpid(child_pid, &exit_stat, 0) == child_pid)
		? exit_stat
		: EXIT_FAILURE;
}
