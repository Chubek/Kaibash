#include "tables.h"
#include "kaibash.h"

struct Atom*
allocate_atom(void)
{
        struct Atom atom = {0};
        return (struct Atom*)allocate_memory(&atom, 1, sizeof(struct Atom));
}

static struct Atom*
atomize_fmt(char* fmt, ...)
{
        va_list ap;
        va_start(ap, fmt);

        struct Atom* atom = allocate_atom();

        if (strstr(fmt, "io-word-here"))
        {
                char* arg  = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = IO_HERE_WORD;
                atom->lnwval_1 = va_arg(ap, size_t);

        }
        else if (strstr(fmt, "cmd-name-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = CMD_NAME_WORD;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "cmd-word-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = CMD_WORD_WORD;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "cmd-suffix-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = CMD_SUFFIX_WORD;
		                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "cmd-prefix-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = CMD_PREFIX_WORD;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "word-list-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = WORD_LIST_WORD;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "pattern-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = PATTERN_WORD;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "case-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = CASE_WORD;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "filename-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->word_kind = FILENAME_WORD;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "escaped-word"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = WORD;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "for-name"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = NAME;
                atom->name_kind = FOR_NAME;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "func-name"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = NAME;
                atom->name_kind = FUNC_NAME;
                atom->nwval_1 = STRDUP(arg);
                atom->lnwval_1 = va_arg(ap, size_t);
        }
        else if (strstr(fmt, "keyword-name"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = NAME;
                atom->name_kind = KEYWORD_NAME;
else if (strstr(fmt, "separator"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = SEPARATOR;
                atom->separator_kind = get_seprator_kind(arg);
        }
        else if (strstr(fmt, "grouper"))
        {
                char* arg = va_arg(ap, char*);
                atom->kind = GROUPER;
                atom->grouper_kind = get_grouper_kind(arg);
        }
        else if (strstr(fmt, "assign-word"))
        {
                char* arg1 = va_arg(ap, char*);
                char* arg2 = va_arg(ap, char*);
                atom->kind = ASSIGNMENT_WORD;
                atom->nwval_1 = arg1;
                atom->nwval_2 = arg2;
                atom->lnwval_1 = va_arg(ap, size_t);
                atom->lnwval_2 = va_arg(ap, size_t);
}
        
        if (strstr(fmt, "param"))
                atom->paramd = true;
        
        if (strstr(fmt, "esc-dquote"))
                atom->escape_kind = ESC_DQUOTE;
        else if (strstr(fmt, "esc-squote"))
                atom->escape_kind = ESC_SQUOTE;
        else if (strstr(fmt, "esc-backtick"))
                atom->escape_kind = ESC_BACKTICK;
        else if (strstr(fmt, "esc-tilde"))
                atom->escape_kind = ESC_TILDE;
        else if (strstr(fmt, "esc-solidus"))
                atom->escape_kind = ESC_SOLIDUS;
        
        if (strstr(fmt, "next-swap"))
        {
                struct Atom** arg = va_arg(ap, struct Atom**);
                atom->next = *arg;
                *arg = atom;
        }
        return atom;
}


