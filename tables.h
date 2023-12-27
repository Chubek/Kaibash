#ifndef TABLES_H_
#define TABLES_H_

struct HeapChain
{
	struct HeapChain* next;
	void* allocation;
	size_t size;
	int tag;
};

struct Symtable
{
 	struct Symtable* next;
	char* symbol;
	void* value;
};

extern struct HeapChain* heap;
extern struct Symtable* symtab;

void allocate_heap_chain(struct HeapChain** chain);
void allocate_symbtable(struct Symtable** table);
void* insert_heap_chain(struct HeapChain** chain, void* data, size_t size, int tag);
void dump_heap(struct HeapChain** chain);
void dump_heap_tag(struct HeapChain** chain, int tag);
void* allocate_memory(size_t size, int tag);
char* duplicate_string(const char* str);
char* get_symbol_value(struct Symtable** table, char* symbol, char* repl);
void insert_symbol(struct Symtable** table, char* symbol, void* value, size_t size);

#endif
