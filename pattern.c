#include "tables.h"
#include "kaibash.h"

struct Pattern*
allocate_pattern(void)
{
	struct Pattern pattern = {0};
	return (struct Pattern*)allocate_memory(&pattern, 1, sizeof(pattern));
}

struct Pattern*
pattern_fmt(char* fmt, ...)
{
	struct Pattern*	pattern = allocate_pattern();

	va_list ap;
	va_start(ap, fmt);

	if (strstr(fmt, "group"))
	{
		pattern->kind	       |= PATT_GROUP;
	}
	if (strstr(fmt, "literal"))
	{
		pattern->kind  	       |= PATT_LITERAL;
		pattern->length 	= va_arg(ap, size_t);
		pattern->value		= va_arg(ap, char*);
	}
	if (strstr(fmt, "regular"))
	{
		pattern->kind	       |= PATT_REGULAR;
		pattern->sigil		= va_arg(ap, enum PatternSigil);
	}
	if (strstr(fmt, "wildcard"))
	{
		pattern->kind	       |= PATT_WILDCARD;
	}

	va_end(ap);

	return pattern;
}

static inline void
rm_literal_prefix(char* word, char* literal)
{
	for (char c1 = *word++, c2 = *literal++;
			c1 == c2 && c2; 
			c1 = *word++, c2 = *literal++);
}

static inline void
rm_literal_suffix(char* word, char* literal, size_t lword, size_t llit)
{
	for (char c1 = *(word + --lword), c2 = *(literal + --llit);
			c1 == c2 && c2 != *literal;
			c1 = *(word + --lword), c2 = *(literal + --llit));
	word = &word[lword];
}

static inline void
rm_epsilon_prefix(char* word, char* epsilon)
{
	for (char c1 = *word; strchr(epsilon, c1) != NULL;)
	{
		for (char c2 = *epsilon++, *p = epsilon; c2; c2 = *epsilon++)
			!(c1 ^ c2)
				? *word++
				: (0);
		epsilon = p;
	}
}


static inline void
rm_epsilon_suffix(char* word, char* epsilon, size_t lword, size_t leps)
{
	for (char c1 = *(word + --lword); strchr(epsilon, c1) != NULL;)
	{
		for (char c2 = *epsilon++, *p = epsilon; c2; c2 = *epsilon++)
			!(c1 ^ c2)
				? --lword
				: (0);
		epsilon = p;
	}
	word = &word[lword];
}
