#include "table.h"
#include "kaibash.h"

static inline struct Sequence*
allocate_sequence(void)
{
	struct Sequence seq = {0};
	return (struct Sequence*)allocate_memory(&seq, 1, sizeof(seq));
}

struct Sequence*
init_sequence(void)
{
	struct Sequence*	seq 	= allocate_sequence();

	seq->process_id		    	= -1;
	seq->group_id			= -1;
	seq->session_id			= -1;
	seq->last_exit_stat		= -1;
}

void
sequence_fmt(Sequence* seq, char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char c;
	int narg = 0;

	for ((c = *fmt++))
	{
		if (c == 'a')
			seq->arguments[narg++] 	= va_arg(ap, Word);
		else if (c == 'p')
			seq->program 		= va_arg(ap, Word);
		else if (c == 'k')
			seq->sequence_kind	= va_arg(ap, enum SequenceKind);
		else if (c == '&')
			seq->parallelized	= true;
		else if (c == '<')
			seq->infile		= va_arg(ap, int);
		else if (c == '>')
			seq->outfile		= va_arg(ap, int);
		else if (c == 'h')
			seq->here_word		= va_arg(ap, word);3
		else
			UNREACHABLE;

	}
}

