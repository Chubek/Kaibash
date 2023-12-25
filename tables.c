#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "tables.h"

extern struct HeapChain* heap;
extern struct Symtable* symtab;

inline void allocate_heap_chain(struct HeapChain** chain)
{
	*chain = (struct HeapChain*) calloc(1, sizeof(**chain));
	(*chain)->next = NULL;
	(*chain)->size = 0;
}


inline void allocate_symbtable(struct Symtable** table)
{
	*table = (struct Symtable*) calloc(1, sizeof(**table));
	(*table)->next = NULL;
	(*table)->symbol = NULL;
	(*table)->value = NULL;
}


inline void* insert_heap_chain(struct HeapChain** chain, void* data, size_t size)
{
	struct HeapChain* node = (struct HeapChain*) calloc(1, sizeof(**chain));
	node->next = *chain;
	node->allocation = (void*) calloc(1, size);
	node->size = size;

	if (data != NULL)
	{
		memmove(node->allocation, data, size);
	}

	*chain = node;

	return node->allocation;
}

inline void dump_heap(struct HeapChain** chain)
{
	struct HeapChain* head = *chain;

	while (head != NULL)
	{
		struct HeapChain* node = head;
		head = head->next;
		free(node->allocation);
		free(node);
	}
}


inline void* allocate_memory(size_t size)
{
	return insert_heap_chain(&heap, NULL, size);
}

inline char* duplicate_string(char* str)
{
	return insert_heap_chain(&heap, str, strlen(str));
}

inline char* get_symbol_value(struct Symtable** table, char* symbol, char* repl)
{
	for (struct Symtable* t = *table; t != NULL; t = t->next)
	{
		if (!strcmp(t->symbol, symbol))
		{
			if (repl != NULL)	
				strcpy(t->value, repl);
			return t->value;
		}
	}

	return NULL;
}

inline void insert_symbol(struct Symtable** table, char* symbol, char* value)
{
        if (get_symbol_value(table, symbol, value) != NULL)
		return;

	struct Symtable* node = (struct Symtable*) allocate_memory(sizeof(**table));
	node->next = *table;
	node->symbol = duplicte_string(symbol);
	node->value = duplicate_string(value);

	*table = node;
}


