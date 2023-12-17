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
