#include "tables.h"
#include "kaibash.h"

static inline struct Pipeline*
allocate_pipeline(void)
{
	struct Pipeline pln 	= {0};
	return (struct Pipeline*)allocate_memory(&pln, 1, sizeof(pln));
}

struct Pipeline*
init_pipeline(struct Sequence* seq, bool banged)
{
	struct Pipeline* pln	= allocate_pipeline();
	
	pln->sequence		= seq;
	pln->banged		= banged;

	return pln;
}

void
run_pipeline(struct Pipeline* pln)
{
	if (pln->next == NULL)
	{
		pln->sequence->infile = pln->pipe_in_fd;
		pln->sequence->outfile = pln->pipe_out_fd;
		pln->sequence->errfile = pln->pipe_err_fd;

		exec_sequence(pln->sequence,
				pln->banged
					? ~(&pln->last_exit_stat)
					: &pln->last_exit_stat);
		return;
	}
	
	if ((pipe(pln->pipe_io)) < 0)
		perror("pipe");

	if (pln->pipe_err)
		if ((pipe(pln->pipe_err)) < 0)
			perror("pipe");

	pid_t child_pid;

	if ((child_pid = fork()) < 0)
		perror("fork");

	if (!child_pid)
	{
		close(pid->pipe_io[1]);
		dup2(pln->pipe_io[0], pln->next->pipe_in_fd);
		close(pln->pipe_io[0]);
		
		if (pln->pipe_err)
		{
			close(pid->pipe_err[1]);
			dup2(pln->pipe_err[0], pln->pipe_fd_err);
			close(pln->pipe_err[0]);
		}

		pln->sequence->infile = pln->next->pipe_in_fd;
		pln->sequence->outfile = pln->next->pipe_out_fd;
		pln->sequence->errfile = pln->next->pipe_err_fd;
	
		exec_sequence(pln->next->sequence,
					pln->next->banged 
						? ~(&pln->next->last_exit_stat)
						: &pln->next->last_exit_stat);
	}
	
	close(pid->pipe_io[0]);
	dup2(pln->pipe_io[1], pln->pipe_out_fd);
	close(pln->pipe_io[1]);
		
	if (pln->pipe_err)
	{
		close(pid->pipe_err[0]);
		dup2(pln->pipe_err[1], pln->pipe_fd_err);
		close(pln->pipe_err[1]);
	}

	pln->sequence->infile = pln->pipe_in_fd;
	pln->sequence->outfile = pln->pipe_out_fd;
	pln->sequence->errfile = pln->pipe_err_fd;
	
	exec_sequence(pln->sequence,
				pln->next->banged 
					? ~(&pln->last_exit_stat)
					: &pln->last_exit_stat);
}
