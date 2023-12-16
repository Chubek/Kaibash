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

	if (strstr(fmt, "wild-card"))
	{
		pattern->pattern_kind 	= PATT_WILDCARD;
	}
	else if (strstr(fmt, "group"))
	{
		pattern->pattern_kind 	= PATT_GROUP;
		pattern->length 	= va_arg(ap, size_t);
		pattern->value     	= va_arg(ap, char*);
	}
	else if (strstr(fmt, "literal"))
	{
		pattern->pattern_kind  	= PATT_LITERAL;
		pattern->length 	= va_arg(ap, size_t);
		pattern->value		= va_arg(ap, char*);
	}
	else if (strstr(fmt, "regular"))
	{
		pattern->pattern_kind	= PATT_REGULAR;
		pattern->length		= va_arg(ap, size_t);
		pattern->value		= va_arg(ap, char*);
		pattern->sigil		= va_arg(ap, enum PatternSigil);

	}

	va_end(ap);

	return pattern;
}
