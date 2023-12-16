#ifndef TABLES_H_
#define TABLES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/*======== Memory allocation table =======*/

// This struct is a value on its own, a singly-linked list, it holds the allocation table
static struct Memory 
{
        struct Memory*  next;
        void*           memv;           // allocated value
        size_t          size;           // its size
        int             nmem;           // number of members
        bool            free;           // whether it's freed (unused)
}
*MEMORY;

// this function is used to allocate memory and get the pointer

void*
allocate_memory(void* value, int nmem, size_t size)
{
        struct Memory* node = calloc(1, sizeof(*MEMORY));
        node->memv          = calloc(nmem, size);
        node->nmem          = nmem;
        node->free          = false;
        node->next          = MEMORY;
        
        memmove(&node->memv, &value, nmem * size);

        MEMORY              = node;
        return node;
}

// This function is used to dump the static allocation table (the allocation table is static, not the allocated values!)
void
dump_memory(void)
{
        for (struct Memory* mem = MEMORY;
                        mem != NULL;
                        mem  = mem->next)
        {
                if (!mem->free)
                {
                        free(mem->memv);
                        mem->free = true;}
        }
}

/*======== Symbols table ========*/


// This is another list structure, it holds the symbols and their values. All symbols are strings
static struct Symtab
{       
        struct Symtab*  next;
        char*           symr;           // symbol representation
        char*           symv;           // symbol value
        size_t          rlen;           // representation length
        size_t          vlen;           // value length
}
*SYMTAB;

// allocate a symbol in the table
void
allocate_symbol(char* symr, char* symv)
{
        size_t lr, lv;
        lr = strlen(symr);
        lv = strlen(symv);
        
        struct Symtab* node = calloc(1, sizeof(*SYMTAB));
        node->symr          = allocate_memory(symr, lr, 1);
        node->symv          = allocate_memory(symv, lv, 1);
        node->rlen          = lr;
        node->vlen          = lv;
        node->next          = SYMTAB;

        SYMTAB              = node;     
}

// get the symbol back
char*
get_symbol(char* symr)
{
        for (struct Symtab* stab = SYMTAB;
                        stab != NULL;
                        stab  = stab->next)
        {
                if (!strncmp(stab->symr, symr, stab->rlen))
                        return stab->symv;
        }
        return NULL;
}

#define STRDUP(s)       allocate_memory(s, 1, strlen(s))

#endif
