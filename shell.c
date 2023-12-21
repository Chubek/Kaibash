#include "tables.h"
#include "kaibash.h"


static inline struct Shell*
allocate_shell(void)
{
	struct Shell sh = {0};
	return (struct Shell*)allocate_memory(&sh, 1, sizeof(sh));
}

struct Shell*
init_shell(bool login)
{
	struct Shell* sh = allocate_shell();

	sh->process_id 	 = getpid();
	sh->group_id     = getgpid();
	sh->session_id   = getsid();
	sh->interactive  = isatty(STDIN_FILENO);
	sh->login	 = login;
}

void
init_pipelines(struct Shell* sh, struct Pipeline* pln_root)
{
	sh->pipelines 	= pln_root;
}

void
init_termfd(struct Shell* sh)
{
	static char cterm_name[L_ctermid + 1] = {0};
	char* cterm_fname = ctermid(&cterm_name[0]);
	sh->termfd	  = open(&cterm_fname[0], O_RDWR | O_EXCL);
}

void 
init_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
}

void 
init_atexit(void)
{
	atexit(dump_memory);
}

void
init_terminal(struct Shell* sh)
{
	tcsetpgrp(sh->termfd, sh->group_id);
	tcgetattr(sh->termfd, &sh->termio);
}
