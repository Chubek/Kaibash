#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "tables.h"

inline void allocate_heap_chain(struct HeapChain** chain)
{
	*chain = (struct HeapChain*) calloc(1, sizeof(**chain));
	(*chain)->next = NULL;
	(*chain)->size = 0;
	(*chain)->tag = -1;
}


inline void allocate_symbtable(struct Symtable** table)
{
	*table = (struct Symtable*) allocate_memory(MEMTAG_SYMTAB, sizeof(**table));
	(*table)->next = NULL;
	(*table)->symbol = NULL;
	(*table)->value = NULL;
}


inline void* insert_heap_chain(struct HeapChain** chain, void* data, size_t size, int tag)
{
	struct HeapChain* node = (struct HeapChain*) calloc(1, sizeof(**chain));
	node->next = *chain;
	node->allocation = (void*) calloc(1, size);
	node->size = size;
	node->tag = tag;

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

inline void dump_heap_tag(struct HeapChain** chain, int tag)
{
	struct HeapChain* head = *chain;

	while (head != NULL)
	{
		struct HeapChain* node = head;
		head = head->next;
		if (!(node->tag & tag))
			free(node)
	}

}

inline void* allocate_memory(size_t size, int tag)
{
	return insert_heap_chain(&heap, NULL, size, tag);
}

inline char* duplicate_string(const char* str)
{
	return insert_heap_chain(&heap, str, strlen(str));
}

inline void* get_symbol_value(struct Symtable** table, char* symbol, void* repl, size_t size)
{
	for (struct Symtable* t = *table; t != NULL; t = t->next)
	{
		if (!strcmp(t->symbol, symbol))
		{
			if (repl != NULL)	
				memcpy(t->value, repl, size);
			return t->value;
		}
	}

	return NULL;
}

inline void insert_symbol(struct Symtable** table, char* symbol, void* value, size_t size)
{
        if (get_symbol_value(table, symbol, value) != NULL)
		return;

	struct Symtable* node = (struct Symtable*) allocate_memory(sizeof(**table));
	node->next = *table;
	node->symbol = symbol;
	node->value = value;

	*table = node;
}


