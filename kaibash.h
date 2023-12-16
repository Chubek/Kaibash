#ifndef KAIBASH_H_
#define KAIBASH_H_

#define ESC_DQUOTE              1
#define ESC_SQUOTE              2
#define ESC_BACKTICK            4
#define ESC_SOLIDUS             8
#define ESC_TILDE               16

typedef char* Word;

struct Atom
{
        struct Atom*    next;

        enum Kind
        {
                ASSIGNMENT_WORD,
                WORD,
                NAME,
                SEPARATOR,
                IO_NUMBER,
                GROUPER,
        }               kind;
enum WordKind
        {
                IO_HERE_WORD,
                CMD_NAME_WORD,
                CMD_WORD_WORD,
                CMD_SUFFIX_WORD,
                CMD_PREFIX_WORD,
                WORD_LIST_WORD,
                PATTERN_WORD,
                CASE_WORD,
                FILENAME_WORD,
                ESC_WORD,
        }               word_kind;
        enum NameKind
        {
                FOR_NAME,
                FUNC_NAME,
                KEYWORD_NAME,
        }               name_kind;
        enum SeparatorKind
        {
                AMPERSAND,
		SEMICOLON,
                DOUBLE_SEMICOLON,
                LESS_AND,
                LESS,
                GREAT,
                GREAT_AND,
                CLOBBER,
                LESS_GREAT,
                DOUBLE_GREAT,
                DOUBLE_LESS,
                DOUBLE_LESS_DASH,
                SHELL_PIPE,
                AND_IF,
                OR_IF,
                PCNT,
                DOUBLE_PCNT,
                POUND,
                DOUBLE_POUND,
        }               separator_kind;
        enum GROUPER_KIND
        {
                PAREN_OPEN,
                PAREN_CLOSE,
                DOBULE_PAREN_OPEN,
                DOUBLE_PAREN_CLOSE,
                CURLY_OPEN,
                CURLY_CLOSE,
                DOUBLE_CURLY_OPEN,
                DOUBLE_CURLY_CLOSED,
        }               grouper_kind;
        enum KEYWORD_KIND
        {
                IF,
                ELIF,
                ELSE,
                FOR,
                CASE,
                ESAC,
                DO,
                DONE,
                WHILE,
                UNTIL,
        }               keyword_kind;

        int             io_num_val;
        Word            nwval_1;
        Word            nwval_2;
        size_t          lnwval_1;
        size_t          lnwval_2;
        bool            paramd;
        int             escape_kind;
};

enum IoMode { INTO, UNTO, CLOBBERER, APPEND, DUP, RW, HERE_STR, HERE_DOC }; 
            /* >     <      >|      >>      >&  <>   <<<        <<        */

#endif
